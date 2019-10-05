open Jest;
open Expect;
module DT = DomTestingLibrary;
module RT = ReactTestingLibrary;

testAsync("useEpicReducer should reduce", finished => {
  let tree = RT.render(<TestEpicReducerComponent />);
  let btn = tree |> RT.getByText(~matcher=`Str("DO SOMETHING"));
  let container = tree |> RT.container
  let options = DT.WaitForElement.makeOptions(~container, ());
  ignore(
    DT.waitForElement(
      ~options,
      ~callback=() => tree |> RT.getByText(~matcher=`Str("DID SOMETHING")),
      (),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve(
           1
           |> expect
           |> toEqual(1)
           |> finished,
         )
       ),
  );
  RT.FireEvent.click(btn);
});

testAsync("useEpicReducer should be epic", finished => {
  let tree = RT.render(<TestEpicReducerComponent />);
  let btn = tree |> RT.getByText(~matcher=`Str("DO SOMETHING"));
  let container = tree |> RT.container
  let options = DT.WaitForElement.makeOptions(~container, ());
  ignore(
    DT.waitForElement(
      ~options,
      ~callback=() => tree |> RT.getByText(~matcher=`Str("DID OTHER THING")),
      (),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve(
           1
           |> expect
           |> toEqual(1)
           |> finished,
         )
       ),
  );
  RT.FireEvent.click(btn);
});