open Jest;

testAsync("Loader renders when hidden", finished =>
  <Loader visible=false /> |> TestUtils.simpleSnapshotTest |> finished
);

testAsync("Loader renders when visible", finished =>
  <Loader visible=true /> |> TestUtils.simpleSnapshotTest |> finished
);