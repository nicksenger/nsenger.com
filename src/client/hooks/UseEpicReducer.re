module WT = Wonka.Types;

type reducerT('s, 'a) = ('s, 'a) => 's;
type epicT('s, 'a) = (WT.sourceT('a), WT.sourceT('s)) => WT.sourceT('a);

let useEpicReducer:
  (reducerT('s, 'a), epicT('s, 'a), 's) => ('s, 'a => unit) =
  (reducer, epic, initialState) => {
    let (state, setState) = React.useState(_ => initialState);
    let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
    let dispatch = a => next(a);
    let newStateStream =
      Wonka.scan((. s, a) => reducer(s, a), initialState, actionStream);
    newStateStream((. signal) =>
      switch (signal) {
      | WT.Push(s) => setState(_s => s)
      | _ => ()
      }
    );
    let epicActionStream = epic(actionStream, newStateStream);
    epicActionStream((. signal) =>
      switch (signal) {
      | WT.Push(a) => dispatch(a)
      | _ => ()
      }
    );
    (state, dispatch);
  };

let combineEpics: list(WT.sourceT('a)) => WT.sourceT('a) =
  actionStreams => {
    let {WT.source, WT.next} = Wonka.makeSubject();
    List.iter(
      s =>
        s((. a) =>
          switch (a) {
          | WT.Push(a) => next(a)
          | _ => ()
          }
        ),
      actionStreams,
    );
    source;
  };