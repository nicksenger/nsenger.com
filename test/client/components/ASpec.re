open Jest;

testAsync("A renders", finished =>
  <A className="foo" href="waldo"> (React.string("barbaz")) </A>
  |> TestUtils.simpleSnapshotTest
  |> finished
);