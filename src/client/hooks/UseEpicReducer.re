type sourceT('a) = Wonka.Types.sourceT('a);
type reducerT('s, 'a) = ('s, 'a) => 's;
type epicT('s, 'a) = (sourceT('a), sourceT('s)) => sourceT('a);

let useEpicReducer:
  (reducerT('s, 'a), epicT('s, 'a), 's) => ('s, 'a => unit) =
  (reducer, epic, initialState) => {
    let (state, setState) = React.useState(_ => initialState);
    let {Wonka.Types.source: actionStream, Wonka.Types.next} =
      Wonka.makeSubject();
    let dispatch = a => next(a);
    let newStateStream =
      Wonka.scan((. s, a) => reducer(s, a), initialState, actionStream);
    newStateStream((. signal) =>
      switch (signal) {
      | Wonka.Types.Push(s) => setState(_s => s)
      | _ => ()
      }
    );
    let epicActionStream = epic(actionStream, newStateStream);
    epicActionStream((. signal) =>
      switch (signal) {
      | Wonka.Types.Push(a) => dispatch(a)
      | _ => ()
      }
    );
    (state, dispatch);
  };