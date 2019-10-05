open Jest;

testAsync("IChing renders all 64 hexagrams", finished =>
  <>
    <div />
    (
      ReasonReact.array(
        Array.of_list(
          List.mapi(
            (i, _) =>
              <IChing
                hexagram=(List.nth(IChing.hexagrams, i))
                key=(string_of_int(i))
              />,
            Belt.List.make(64, ()),
          ),
        ),
      )
    )
  </>
  |> TestUtils.simpleSnapshotTest
  |> finished
);