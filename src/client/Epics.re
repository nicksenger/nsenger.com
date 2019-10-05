module WT = Wonka.Types;

let submitMessageEpic = (actionStream, fetch, status) =>
  actionStream
  |> Wonka.filter((. a) =>
       switch (a) {
       | Types.SubmitMessageRequest(_, _) => true
       | _ => false
       }
     )
  |> Wonka.mergeMap((. a) =>
       switch (a) {
       | Types.SubmitMessageRequest(email, message) =>
         let payload = Js.Dict.empty();
         Js.Dict.set(payload, "email", Js.Json.string(email));
         Js.Dict.set(payload, "message", Js.Json.string(message));
         Wonka.fromPromise(
           fetch(
             "/send-message",
             Fetch.RequestInit.make(
               ~method_=Post,
               ~body=
                 Fetch.BodyInit.make(
                   Js.Json.stringify(Js.Json.object_(payload)),
                 ),
               ~headers=
                 Fetch.HeadersInit.make({"Content-Type": "application/json"}),
               (),
             ),
           ),
         )
         |> Wonka.map((. r) =>
              switch (status(r)) {
              | 200 => Types.SubmitMessageSuccess
              | _ => Types.SubmitMessageFailure
              }
            );
       | _ => Wonka.fromList([])
       }
     );

let submitMessageCompletionEpic = (actionStream, push) =>
  actionStream
  |> Wonka.filter((. a) =>
       switch (a) {
       | Types.SubmitMessageSuccess =>
         push("/contact?success=true");
         false;
       | Types.SubmitMessageFailure =>
         push("/contact?success=false");
         false;
       | _ => false
       }
     );

let rootEpic = (actionStream, _stateStream) =>
  UseEpicReducer.combineEpics([
    submitMessageEpic(
      actionStream,
      Fetch.fetchWithInit,
      Fetch.Response.status,
    ),
    submitMessageCompletionEpic(actionStream, ReasonReactRouter.push),
  ]);