let logLatestState =
  (. (a: Types.submissionAction, s: Types.submissionState)) => {
    Js.log(s);
    (a, s);
  };

let combineEpics: list(Wonka.Types.sourceT('a)) => Wonka.Types.sourceT('a) =
  actionStreams => {
    let {Wonka.Types.source, Wonka.Types.next} = Wonka.makeSubject();
    List.iter(
      s =>
        s((. a) =>
          switch (a) {
          | Wonka.Types.Push(a) => next(a)
          | _ => ()
          }
        ),
      actionStreams,
    );
    source;
  };

let logger = (actionStream, _stateStream) =>
  actionStream
  |> Wonka.filter((. a) =>
       switch (a) {
       | Types.SubmitMessageRequest(_, _) => true
       | Types.SubmitMessageSuccess => true
       | _ => false
       }
     )
  |> Wonka.filter((. a) =>
       switch (a) {
       | Types.SubmitMessageRequest(_, _) => true
       | _ => false
       }
     )
  |> Wonka.delay(2000)
  |> Wonka.map((. _) => Types.SubmitMessageSuccess);

let rootEpic = (actionStream, stateStream) =>
  combineEpics([logger(actionStream, stateStream)]);