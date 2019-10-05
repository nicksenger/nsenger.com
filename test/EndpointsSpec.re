open Jest;
open Expect;

testAsync("makeSuccessJson", finished => {
  let expectedObj = Js.Dict.empty();
  Js.Dict.set(expectedObj, "success", Js.Json.boolean(true));
  let expected = Js.Json.object_(expectedObj);
  Endpoints.makeSuccessJson() |> expect |> toEqual(expected) |> finished;
});

testAsync("sendEmail data", finished => {
  let expected =
    Mailgun.emailData(
      ~from={j|Senger.io Visitor <foo@bar.com>|j},
      ~to_="dev@nsenger.com",
      ~subject="Inquiry",
      ~text="testtest",
    );
  let send = (_a, data, c) => {
    data |> expect |> toEqual(expected) |> finished;
    c(Belt_Result.Ok());
  };
  ignore(Endpoints.sendMail("foo@bar.com", "testtest", send));
});

testAsync("sendEmail success", finished => {
  let send = (_a, _b, c) => c(Belt_Result.Ok());
  ignore(
    Endpoints.sendMail("foo@bar.com", "testtest", send)
    |> Js.Promise.then_(r =>
         Js.Promise.resolve(
           r |> expect |> toEqual(Endpoints.makeSuccessJson()) |> finished,
         )
       ),
  );
});

testAsync("sendEmail failure", finished => {
  let send = (_a, _b, c) => c(Belt_Result.Error());
  ignore(
    Endpoints.sendMail("foo@bar.com", "testtest", send)
    |> Js.Promise.then_(_ => Js.Promise.resolve())
    |> Js.Promise.catch(_ =>
         Js.Promise.resolve(1 |> expect |> toEqual(1) |> finished)
       ),
  );
});

testAsync("parseMessageBody no body", finished =>
  Endpoints.parseMessageBody(None) |> expect |> toEqual(None) |> finished
);

testAsync("parseMessageBody no email", finished => {
  let bodyDict = Js.Dict.empty();
  ignore(Js.Dict.set(bodyDict, "message", Js.Json.string("test")));
  let body = Js.Json.object_(bodyDict);
  Endpoints.parseMessageBody(Some(body))
  |> expect
  |> toEqual(None)
  |> finished;
});

testAsync("parseMessageBody no message", finished => {
  let bodyDict = Js.Dict.empty();
  ignore(Js.Dict.set(bodyDict, "email", Js.Json.string("foo@bar.com")));
  let body = Js.Json.object_(bodyDict);
  Endpoints.parseMessageBody(Some(body))
  |> expect
  |> toEqual(None)
  |> finished;
});

testAsync("parseMessageBody OK", finished => {
  let bodyDict = Js.Dict.empty();
  ignore(Js.Dict.set(bodyDict, "email", Js.Json.string("foo@bar.com")));
  ignore(Js.Dict.set(bodyDict, "message", Js.Json.string("test")));
  let body = Js.Json.object_(bodyDict);
  Endpoints.parseMessageBody(Some(body))
  |> expect
  |> toEqual(
       Some((Js.Json.string("foo@bar.com"), Js.Json.string("test"))),
     )
  |> finished;
});