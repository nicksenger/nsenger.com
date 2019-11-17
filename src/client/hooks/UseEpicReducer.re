module WT = Wonka.Types;

/* Identical to a reducer one would provide to Redux's `createStore` or React's `useReducer`
   hook. This will determine the new state based on the previous state and a dispatched action. */
type reducerT('s, 'a) = ('s, 'a) => 's;

/* An `epic` here is a function of the action and state streams which returns a new
   action stream. This is provided to `useEpicReducer` to orchestrate any sort of
   side effects, see: https://redux-observable.js.org/docs/basics/Epics.html */
type epicT('s, 'a) = (WT.sourceT('a), WT.sourceT('s)) => WT.sourceT('a);

/* Given a reducer, an epic for handling side side effects, and an initial state, returns the state
   and dispatch function. Dispatched actions will first be used by the reducer to calculate the new
   state, after which they will be processed by the provided epic. Any new actions emitted from
   the epic are dispatched and processed in the same way. */
let useEpicReducer:
  (reducerT('s, 'a), epicT('s, 'a), 's) => ('s, 'a => unit) =
  (reducer, epic, initialState) => {
    /* Set up the component/application state */
    let (state, setState) = React.useState(_ => initialState);

    /* Memoize creation of streams. Note that if the identity of `reducer`, `epic`, or `initialState` is changed,
       then the state will be reset. Generally these dependencies should remain constant for a given application. */
    let dispatch =
      React.useMemo3(
        _ => {
          /* Set up the main action stream */
          let {WT.source: actionStream, WT.next} = Wonka.makeSubject();

          /* Set up the state stream, which emits a new state for every action emitted from the main action stream */
          let newStateStream =
            Wonka.scan(
              (. s, a) => reducer(s, a),
              initialState,
              actionStream,
            );

          /* Always set the state equal to the latest emission from the state stream */
          newStateStream((. signal) =>
            switch (signal) {
            | WT.Push(s) => setState(_s => s)
            | _ => ()
            }
          );

          /* Invoke the provided epic with the action and state streams to get the epic action stream */
          let epicActionStream = epic(actionStream, newStateStream);

          /* Pipe actions from the epic action stream to the main action stream */
          epicActionStream((. signal) =>
            switch (signal) {
            | WT.Push(a) => next(a)
            | _ => ()
            }
          );

          /* Return the dispatch function, which just emits the provided action from the action stream */
          a => next(a);
        },
        (reducer, epic, initialState),
      );

    /* Return the state and dispatch function for consumption in the component tree */
    (state, dispatch);
  };

/* Combines a list of epics into a single epic. Note that the provided order of
   epics is also the order in which they will receive actions, see:
   https://redux-observable.js.org/docs/api/combineEpics.html */
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