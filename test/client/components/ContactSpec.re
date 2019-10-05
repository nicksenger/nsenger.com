open Jest;
open Expect;
module DT = DomTestingLibrary;
module RT = ReactTestingLibrary;

testAsync("Contact renders hidden pending", finished =>
  <Contact visible=false status=Types.Pending onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders hidden successful", finished =>
  <Contact visible=false status=Types.Success onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders hidden failed", finished =>
  <Contact visible=false status=Types.Failed onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders visible pending", finished =>
  <Contact visible=true status=Types.Pending onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders visible successful", finished =>
  <Contact visible=true status=Types.Success onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders visible failed", finished =>
  <Contact visible=true status=Types.Failed onSubmit=((_, _) => ()) />
  |> TestUtils.simpleSnapshotTest
  |> finished
);

testAsync("Contact renders invalid email", finished => {
  let tree =
    RT.render(
      <Contact visible=true status=Types.Pending onSubmit=((_, _) => ()) />,
    );
  let input = tree |> RT.getByLabelText(~matcher=`Str("Return Email"));
  let container = tree |> RT.container;
  let options = DT.WaitForElement.makeOptions(~container, ());
  ignore(
    DT.waitForElement(
      ~options,
      ~callback=() => tree |> RT.getByText(~matcher=`Str("SUBMIT")),
      (),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve(
           tree
           |> RT.getByText(~matcher=`Str("SUBMIT"))
           |> expect
           |> JestDom.toBeDisabled
           |> finished,
         )
       ),
  );
  RT.FireEvent.change(input, ~eventInit={
                               "target": {
                                 "value": "foo",
                               },
                             });
});

testAsync("Contact renders invalid message", finished => {
  let tree =
    RT.render(
      <Contact visible=true status=Types.Pending onSubmit=((_, _) => ()) />,
    );
  let input = tree |> RT.getByLabelText(~matcher=`Str("Message"));
  let container = tree |> RT.container;
  let options = DT.WaitForElement.makeOptions(~container, ());
  let btnChange = () => tree |> RT.getByText(~matcher=`Str("SUBMIT"));
  ignore(
    DT.waitForElement(~options, ~callback=btnChange, ())
    |> Js.Promise.then_(_ => {
         RT.FireEvent.change(input, ~eventInit={
                                      "target": {
                                        "value": "",
                                      },
                                    });
         DT.waitForElement(~options, ~callback=btnChange, ())
         |> Js.Promise.then_(_ =>
              Js.Promise.resolve(
                tree
                |> RT.getByText(~matcher=`Str("SUBMIT"))
                |> expect
                |> JestDom.toBeDisabled
                |> finished,
              )
            );
       }),
  );
  RT.FireEvent.change(input, ~eventInit={
                               "target": {
                                 "value": "foobar!",
                               },
                             });
});