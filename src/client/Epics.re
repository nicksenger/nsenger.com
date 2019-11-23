module WT = Wonka.Types;

/* Handle submitting a message */
let submitMessageEpic = (actionStream, fetch, status) =>
  actionStream
  /* We're only concerned with performing this side effect when actions of the type `SubmitMessageRequest`
     are dispatched, so first filter out any other actions */
  |> Wonka.filter((. a) =>
       switch (a) {
       | Types.SubmitMessageRequest(_, _) => true
       | _ => false
       }
     )
  /* If somehow the user dispatches another message before the first request has finished, we no longer
     care about updating the UI based on the result of the first request. This shouldn't be possible based
     on how the UI is written, so `mergeMap` would achieve the same thing here.
     See https://github.com/kitten/wonka/blob/master/docs/api/operators.md */
  |> Wonka.switchMap((. a)
       /* `a` is guaranteed to be of type `SubmitMessageRequest` but I don't know how to communicate that to
          the compiler in this case, so this switch is a hacky work-around. */
       =>
         switch (a) {
         | Types.SubmitMessageRequest(email, message) =>
           /* Make the payload for the user's message */
           let payload = Js.Dict.empty();
           Js.Dict.set(payload, "email", Js.Json.string(email));
           Js.Dict.set(payload, "message", Js.Json.string(message));

           /* Make the inner stream from a request promise to the api with our message payload */
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
                   Fetch.HeadersInit.make({
                     "Content-Type": "application/json",
                   }),
                 (),
               ),
             ),
           )
           /* If the response is successful, emit the success message, otherwise emit the failure message.
           `switchMap` will emit these actions from the outer stream, so overall we are taking a stream
           of actions in and returning a stream of either `SubmitMessageSuccess` or `SubmitMessageFailure`
           actions. */
           |> Wonka.map((. r) =>
                switch (status(r)) {
                | 200 => Types.SubmitMessageSuccess
                | _ => Types.SubmitMessageFailure
                }
              );
         /* For other types of actions return a no-op inner stream. It shouldn't be possible to hit this case */
         | _ => Wonka.fromList([])
         }
       );

/* Update the URL based on the outcome of the message submission */
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

/* The root epic pipes the emissions from the action stream to all of the epics in the application.
   This is what will be passed to `useEpicReducer`. */
let rootEpic = (actionStream, _stateStream) =>
  /* `combineEpics` is used here to squash the 2 epics into a single epic */
  UseEpicReducer.combineEpics([
    submitMessageEpic(
      actionStream,
      Fetch.fetchWithInit,
      Fetch.Response.status,
    ),
    submitMessageCompletionEpic(actionStream, ReasonReactRouter.push),
  ]);