open Jest;

testAsync("YiLoader renders at size 20 and 100", finished =>
  <> <YiLoader size=20 /> <YiLoader size=100 /> </>
  |> TestUtils.simpleSnapshotTest
  |> finished
);