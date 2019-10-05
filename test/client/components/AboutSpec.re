open Jest;

testAsync("About renders visible", finished =>
  <About visible=true /> |> TestUtils.simpleSnapshotTest |> finished
);

testAsync("About renders not visible", finished =>
  <About visible=false /> |> TestUtils.simpleSnapshotTest |> finished
);