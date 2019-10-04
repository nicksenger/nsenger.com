open Jest;

testAsync("Header renders", finished =>
  <Header toggleMenu=(() => ()) /> |> TestUtils.simpleSnapshotTest |> finished
);