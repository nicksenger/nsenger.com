open Jest;

testAsync("Menu renders", finished =>
  <Menu toggleMenu=(() => ()) /> |> TestUtils.simpleSnapshotTest |> finished
);