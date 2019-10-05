open Jest;

testAsync("Footer renders menu open", finished =>
  <Footer route=Types.Home menuOpen=Types.Open />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Footer renders menu closed", finished =>
  <Footer route=Types.Home menuOpen=Types.Closed />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Footer renders on about page", finished =>
  <Footer route=Types.About menuOpen=Types.Open />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Footer renders on contact page", finished =>
  <Footer route=Types.About menuOpen=Types.Open />
  |> TestUtils.simpleSnapshotTest
  |> finished
);