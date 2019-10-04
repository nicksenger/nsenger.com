open Jest;
open Expect;
module WT = Wonka.Types;

type combinableAction =
  | A
  | B
  | C;

testAsync("combining epics", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let aWatcher =
    actionStream
    |> Wonka.filter((. a) =>
         switch (a) {
         | A => true
         | _ => false
         }
       )
    |> Wonka.map((. _) => B);
  let bWatcher =
    actionStream
    |> Wonka.filter((. a) =>
         switch (a) {
         | B => true
         | _ => false
         }
       )
    |> Wonka.map((. _) => C);
  let cWatcher =
    actionStream
    |> Wonka.filter((. a) =>
         switch (a) {
         | C =>
           ignore(1 |> expect |> toEqual(1) |> finished);
           false;
         | _ => false
         }
       );

  let combined = Epics.combineEpics([aWatcher, bWatcher, cWatcher]);
  combined((. signal) =>
    switch (signal) {
    | WT.Push(a) => next(a)
    | _ => ()
    }
  );
  next(A);
});

testAsync("the submit message completion", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let {WT.source: stateStream, _} = Wonka.makeSubject();
  let push = s =>
    switch (s) {
    | "/contact?success=true" =>
      ignore(1 |> expect |> toEqual(1) |> finished)
    | _ => ()
    };

  Epics.submitMessageCompletionEpic(actionStream, stateStream, push);
  next(Types.SubmitMessageSuccess);
});