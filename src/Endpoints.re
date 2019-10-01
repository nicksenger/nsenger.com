exception ApiError;

let mailgunKeys =
  Mailgun.mailgunReq(
    ~apiKey=Js.Dict.unsafeGet(Node.Process.process##env, "MAILGUN_API_KEY"),
    ~domain=Js.Dict.unsafeGet(Node.Process.process##env, "MAILGUN_DOMAIN"),
  );
let mailgun = Mailgun.mailgun(mailgunKeys);
let messages = Mailgun.Message.messages(mailgun);

let makeSuccessJson = () => {
  let json = Js.Dict.empty();
  Js.Dict.set(json, "success", Js.Json.boolean(true));
  Js.Json.object_(json);
};

let sendMail = (email, message) => {
  let emailData =
    Mailgun.emailData(
      ~from={j|Senger.io Visitor <$email>|j},
      ~to_="dev@nsenger.com",
      ~subject="Inquiry",
      ~text={j|$message|j},
    );
  Js.Promise.make((~resolve, ~reject) =>
    Mailgun.Message.send(messages, emailData, b =>
      switch (b) {
      | Ok(_) => resolve(. makeSuccessJson())
      | Error(_) => reject(. ApiError)
      }
    )
  );
};

let badRequest = res =>
  res
  |> Express.Response.sendStatus(Express.Response.StatusCode.BadRequest)
  |> Js.Promise.resolve;

let methodNotAllowed = res =>
  res
  |> Express.Response.sendStatus(
       Express.Response.StatusCode.MethodNotAllowed,
     )
  |> Js.Promise.resolve;

let getDictString = (dict, key) =>
  switch (Js.Dict.get(dict, key)) {
  | Some(json) => Js.Json.decodeString(json)
  | _ => None
  };

let parseBody = body =>
  switch (body) {
  | Some(bj) =>
    switch (Js.Json.decodeObject(bj)) {
    | Some(b) =>
      let e = Js.Dict.get(b, "email");
      let m = Js.Dict.get(b, "message");
      switch (e) {
      | Some(_) =>
        switch (m) {
        | Some(_) => Some((e, m))
        | _ => None
        }
      | _ => None
      };
    | _ => None
    }
  | _ => None
  };

let redirect = (res, success) =>
  res
  |> Express.Response.redirect({j|/contact?success=$success|j})
  |> Js.Promise.resolve;

let sendMessageNoJs = (_next, req, res) =>
  switch (Express.Request.httpMethod(req)) {
  | Express.Request.Post =>
    let body = Express.Request.bodyJSON(req);
    switch (parseBody(body)) {
    | Some((e, m)) =>
      switch (e) {
      | Some(s) =>
        switch (Contact.isValidEmail(Js.Json.decodeString(s))) {
        | false => redirect(res, "false")
        | _ =>
          switch (m) {
          | Some(message) =>
            switch (Contact.isValidMessage(Js.Json.decodeString(message))) {
            | true =>
              sendMail(e, m) |> Js.Promise.then_(_ => redirect(res, "true"))
            | _ => redirect(res, "false")
            }
          | _ => redirect(res, "false")
          }
        }
      | _ => redirect(res, "false")
      }
    | None => redirect(res, "false")
    };
  | _ => redirect(res, "false")
  };

let sendMessage = (_next, req, res) =>
  switch (Express.Request.httpMethod(req)) {
  | Express.Request.Post =>
    let body = Express.Request.bodyJSON(req);
    switch (parseBody(body)) {
    | Some((e, m)) =>
      sendMail(e, m)
      |> Js.Promise.then_(responseMessage =>
           res
           |> Express.Response.sendJson(responseMessage)
           |> Js.Promise.resolve
         )
    | None => badRequest(res)
    };
  | _ => methodNotAllowed(res)
  };