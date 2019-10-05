open Jest;
open Expect;
module WT = Wonka.Types;
module DT = DomTestingLibrary;
module RT = ReactTestingLibrary;

testAsync("useEpicReducer should reduce", finished => {
  let tree = RT.render(<TestEpicReducerComponent />);
  let btn = tree |> RT.getByText(~matcher=`Str("DO SOMETHING"));
  let container = tree |> RT.container
  let options = DT.WaitForElement.makeOptions(~container, ());
  ignore(
    DT.waitForElement(
      ~options,
      ~callback=() => tree |> RT.getByText(~matcher=`Str("DID SOMETHING")),
      (),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve(
           1
           |> expect
           |> toEqual(1)
           |> finished,
         )
       ),
  );
  RT.FireEvent.click(btn);
});

testAsync("useEpicReducer should be epic", finished => {
  let tree = RT.render(<TestEpicReducerComponent />);
  let btn = tree |> RT.getByText(~matcher=`Str("DO SOMETHING"));
  let container = tree |> RT.container
  let options = DT.WaitForElement.makeOptions(~container, ());
  ignore(
    DT.waitForElement(
      ~options,
      ~callback=() => tree |> RT.getByText(~matcher=`Str("DID OTHER THING")),
      (),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve(
           1
           |> expect
           |> toEqual(1)
           |> finished,
         )
       ),
  );
  RT.FireEvent.click(btn);
});

type combinableAction =
  | A
  | B
  | C;

testAsync("combineEpics", finished => {
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

  let combined = UseEpicReducer.combineEpics([aWatcher, bWatcher, cWatcher]);
  combined((. signal) =>
    switch (signal) {
    | WT.Push(a) => next(a)
    | _ => ()
    }
  );
  next(A);
});