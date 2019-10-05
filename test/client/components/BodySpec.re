open Jest;

testAsync("Body renders the home page", finished => {
  let url: ReasonReactRouter.url = {path: ["home"], hash: "", search: ""};
  <Body serverUrl=(Some(url)) /> |> TestUtils.simpleSnapshotTest |> finished;
});

testAsync("Body renders the about page", finished => {
  let url: ReasonReactRouter.url = {path: ["about"], hash: "", search: ""};
  <Body serverUrl=(Some(url)) /> |> TestUtils.simpleSnapshotTest |> finished;
});

testAsync("Body renders the contact page", finished => {
  let url: ReasonReactRouter.url = {path: ["contact"], hash: "", search: ""};
  <Body serverUrl=(Some(url)) /> |> TestUtils.simpleSnapshotTest |> finished;
});