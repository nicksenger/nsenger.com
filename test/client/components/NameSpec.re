open Jest;

testAsync("Name renders when hidden", finished =>
  <Name visible=false /> |> TestUtils.simpleSnapshotTest |> finished
);

testAsync("Name renders when visible", finished =>
  <Name visible=true /> |> TestUtils.simpleSnapshotTest |> finished
);