type action =
  | DoSomething
  | DoOtherThing;

type state = {
  didSomething: bool,
  didOtherThing: bool,
};

let reducer: (state, action) => state =
  (s, a) =>
    switch (a) {
    | DoSomething => {didSomething: true, didOtherThing: s.didOtherThing}
    | DoOtherThing => {
        didSomething: s.didSomething,
        didOtherThing: true,
      }
    };

let epic = (actionStream, _) =>
  actionStream
  |> Wonka.filter((. a) =>
       switch (a) {
       | DoSomething => true
       | _ => false
       }
     )
  |> Wonka.map((. _) => DoOtherThing);

let initialState: state = {didSomething: false, didOtherThing: false};

[@react.component]
let make = () => {
  let (state, dispatch) = Hooks.useEpicReducer(reducer, epic, initialState);
  <>
    (
      switch (state.didSomething) {
      | true => <h3> (React.string("DID SOMETHING")) </h3>
      | _ => React.null
      }
    )
    (
      switch (state.didOtherThing) {
      | true => <h3> (React.string("DID OTHER THING")) </h3>
      | _ => React.null
      }
    )
    <button onClick=(_ => dispatch(DoSomething))>
      (React.string("DO SOMETHING"))
    </button>
  </>;
};