open Jest;
open Expect;
module WT = Wonka.Types;

testAsync("submitting the message should fetch to message URI", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let fetch = (path, _init) =>
    switch (path) {
    | "/send-message" =>
      ignore(1 |> expect |> toEqual(1) |> finished) |> Js.Promise.resolve
    | _ => () |> Js.Promise.resolve
    };
  let status = _r => 200;

  let epic = Epics.submitMessageEpic(actionStream, fetch, status);
  epic((. _) => ());
  next(Types.SubmitMessageRequest("foo@bar.com", "testtest"));
});

testAsync("submitting the message successfully should dispatch success action", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let fetch = (_path, _init) => () |> Js.Promise.resolve;
  let status = _r => 200;

  let epic = Epics.submitMessageEpic(actionStream, fetch, status);
  epic((. signal) =>
    switch (signal) {
    | WT.Push(a) =>
      switch (a) {
      | Types.SubmitMessageSuccess => ignore(1 |> expect |> toEqual(1) |> finished)
      | _ => ()
      }
    | _ => ()
    }
  );
  next(Types.SubmitMessageRequest("foo@bar.com", "testtest"));
});



testAsync("failing to submit the message should dispatch failure action", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let fetch = (_path, _init) => () |> Js.Promise.resolve;
  let status = _r => 400;

  let epic = Epics.submitMessageEpic(actionStream, fetch, status);
  epic((. signal) =>
    switch (signal) {
    | WT.Push(a) =>
      switch (a) {
      | Types.SubmitMessageFailure => ignore(1 |> expect |> toEqual(1) |> finished)
      | _ => ()
      }
    | _ => ()
    }
  );
  next(Types.SubmitMessageRequest("foo@bar.com", "testtest"));
});

testAsync("submitMessageCompletion success", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let push = s =>
    switch (s) {
    | "/contact?success=true" =>
      ignore(1 |> expect |> toEqual(1) |> finished)
    | _ => ()
    };

  let epic = Epics.submitMessageCompletionEpic(actionStream, push);
  epic((. _) => ());
  next(Types.SubmitMessageSuccess);
});

testAsync("submitMessageCompletion failure", finished => {
  let {WT.source: actionStream, WT.next} = Wonka.makeSubject();
  let push = s =>
    switch (s) {
    | "/contact?success=false" =>
      ignore(1 |> expect |> toEqual(1) |> finished)
    | _ => ()
    };

  let epic = Epics.submitMessageCompletionEpic(actionStream, push);
  epic((. _) => ());
  next(Types.SubmitMessageFailure);
});