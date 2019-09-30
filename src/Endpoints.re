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

let getDictString = (dict, key) =>
  switch (Js.Dict.get(dict, key)) {
  | Some(json) => Js.Json.decodeString(json)
  | _ => None
  };

let sendMessage = (_next, req, res) =>
  switch (Express.Request.httpMethod(req)) {
  | Express.Request.Post =>
    let body = Express.Request.bodyJSON(req);
    switch (body) {
    | Some(bj) =>
      switch (Js.Json.decodeObject(bj)) {
      | Some(b) =>
        let e = Js.Dict.get(b, "email");
        let m = Js.Dict.get(b, "message");
        switch (e) {
        | Some(_) =>
          switch (m) {
          | Some(_) =>
            sendMail(e, m)
            |> Js.Promise.then_(responseMessage =>
                 res
                 |> Express.Response.sendJson(responseMessage)
                 |> Js.Promise.resolve
               )
          | _ => badRequest(res)
          }
        | _ => badRequest(res)
        };
      | _ => badRequest(res)
      }
    | _ => badRequest(res)
    };
  | _ =>
    res
    |> Express.Response.sendStatus(
         Express.Response.StatusCode.MethodNotAllowed,
       )
    |> Js.Promise.resolve
  };