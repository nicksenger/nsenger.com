[@bs.config {jsx: 3}];

/* State declaration */
type state = {
  count: int,
  show: bool,
};

/* Action declaration */
type action =
  | Click
  | Toggle;

/* greeting and children are props. `children` isn't used, therefore ignored.
   We ignore it by prepending it with an underscore */
[@react.component]
let make = (~greeting: string) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Click => {...state, count: state.count + 1}
        | Toggle => {...state, show: !state.show}
        },
      {count: 0, show: true},
    );

  let message = "You've clicked this " ++ string_of_int(state.count) ++ " times(s)";
  <div>
    <button onClick={_event => dispatch(Click)}>
      {ReasonReact.string(message)}
    </button>
    <button onClick={_event => dispatch(Toggle)}>
      {ReasonReact.string("Toggle greeting")}
    </button>
    {state.show ? ReasonReact.string(greeting) : ReasonReact.null}
  </div>
};
