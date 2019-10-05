exception ApiError;

let mailgunKeys =
  Mailgun.mailgunReq(
    ~apiKey=
      switch (Js.Dict.get(Node.Process.process##env, "MAILGUN_API_KEY")) {
      | Some(s) => s
      | None => "foo"
      },
    ~domain=
      switch (Js.Dict.get(Node.Process.process##env, "MAILGUN_DOMAIN")) {
      | Some(s) => s
      | None => "bar"
      },
  );
let mailgun = Mailgun.mailgun(mailgunKeys);
let messages = Mailgun.Message.messages(mailgun);

let makeSuccessJson = () => {
  let json = Js.Dict.empty();
  Js.Dict.set(json, "success", Js.Json.boolean(true));
  Js.Json.object_(json);
};

let sendMail = (email, message, send) => {
  let emailData =
    Mailgun.emailData(
      ~from={j|Senger.io Visitor <$email>|j},
      ~to_="dev@nsenger.com",
      ~subject="Inquiry",
      ~text={j|$message|j},
    );
  Js.Promise.make((~resolve, ~reject) =>
    send(messages, emailData, b =>
      switch (b) {
      | Belt_Result.Ok(_) => resolve(. makeSuccessJson())
      | Belt_Result.Error(_) => reject(. ApiError)
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

let redirect = (res, success) =>
  res
  |> Express.Response.redirect({j|/contact?success=$success|j})
  |> Js.Promise.resolve;

let parseMessageBody = body =>
  switch (body) {
  | Some(bj) =>
    switch (Js.Json.decodeObject(bj)) {
    | Some(b) =>
      let e = Js.Dict.get(b, "email");
      let m = Js.Dict.get(b, "message");
      switch (e) {
      | Some(email) =>
        switch (m) {
        | Some(message) => Some((email, message))
        | _ => None
        }
      | _ => None
      };
    | _ => None
    }
  | _ => None
  };

let sendMessageNoJs = (_next, req, res) =>
  switch (Express.Request.httpMethod(req)) {
  | Express.Request.Post =>
    let body = Express.Request.bodyJSON(req);
    switch (parseMessageBody(body)) {
    | Some((email, message)) =>
      switch (Contact.isValidEmail(Js.Json.decodeString(email))) {
      | false => redirect(res, "false")
      | _ =>
        switch (Contact.isValidMessage(Js.Json.decodeString(message))) {
        | true =>
          sendMail(email, message, Mailgun.Message.send)
          |> Js.Promise.then_(_ => redirect(res, "true"))
        | _ => redirect(res, "false")
        }
      }
    | None => redirect(res, "false")
    };
  | _ => redirect(res, "false")
  };

let sendMessage = (_next, req, res) =>
  switch (Express.Request.httpMethod(req)) {
  | Express.Request.Post =>
    let body = Express.Request.bodyJSON(req);
    switch (parseMessageBody(body)) {
    | Some((email, message)) =>
      sendMail(email, message, Mailgun.Message.send)
      |> Js.Promise.then_(responseMessage =>
           res
           |> Express.Response.sendJson(responseMessage)
           |> Js.Promise.resolve
         )
    | None => badRequest(res)
    };
  | _ => methodNotAllowed(res)
  };