open Jest;
open Expect;
open ReactTestingLibrary;

let simpleSnapshotTest = tree =>
  tree |> render |> container |> expect |> toMatchSnapshot;