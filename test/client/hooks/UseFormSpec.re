open Jest;
open Expect;
module DT = DomTestingLibrary;
module RT = ReactTestingLibrary;

testAsync("useForm no errors with empty initial", finished => {
  let tree = RT.render(<TestFormComponent />);
  tree
  |> RT.getByText(~matcher=`Str("SUBMIT"))
  |> expect
  |> JestDom.toBeEnabled
  |> finished;
});

testAsync("useForm no error when change input to valid", finished => {
  let tree = RT.render(<TestFormComponent />);
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 1"));
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
           |> JestDom.toBeEnabled
           |> finished,
         )
       ),
  );
  RT.FireEvent.change(input, ~eventInit={
                               "target": {
                                 "value": "bar",
                               },
                             });
});

testAsync("useForm error when change first input to invalid", finished => {
  let tree = RT.render(<TestFormComponent />);
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 1"));
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

testAsync("useForm error when change second input to invalid", finished => {
  let tree = RT.render(<TestFormComponent />);
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 2"));
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
                                 "value": "bar",
                               },
                             });
});

testAsync(
  "useForm no error when change input to invalid if no validate on change",
  finished => {
  let tree = RT.render(<TestFormComponent validateOnChange=false />);
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 1"));
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
           |> JestDom.toBeEnabled
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

testAsync("useForm error when blur input if invalid", finished => {
  let tree = RT.render(<TestFormComponent validateOnChange=false />);
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 1"));
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
  RT.FireEvent.blur(input, ~eventInit={
                             "target": {
                               "value": "foo",
                             },
                           });
});

testAsync(
  "useForm no error when blur input invalid if not validating on blur",
  finished => {
  let tree =
    RT.render(
      <TestFormComponent validateOnChange=false validateOnBlur=false />,
    );
  let input = tree |> RT.getByLabelText(~matcher=`Str("INPUT 1"));
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
           |> JestDom.toBeEnabled
           |> finished,
         )
       ),
  );
  RT.FireEvent.blur(input, ~eventInit={
                             "target": {
                               "value": "foo",
                             },
                           });
});

testAsync("useForm no error if initial values invalid", finished => {
  let initial = Js.Dict.empty();
  ignore(Js.Dict.set(initial, "input1", "foo"));
  ignore(Js.Dict.set(initial, "input2", "bar"));
  ignore(Js.Dict.set(initial, "input3", "baz"));
  let tree = RT.render(<TestFormComponent initialValues=initial />);
  tree
  |> RT.getByText(~matcher=`Str("SUBMIT"))
  |> expect
  |> JestDom.toBeEnabled
  |> finished;
});

testAsync("useForm should validate all inputs on validate()", finished => {
  let initial = Js.Dict.empty();
  ignore(Js.Dict.set(initial, "input1", "foo"));
  ignore(Js.Dict.set(initial, "input2", "bar"));
  ignore(Js.Dict.set(initial, "input3", "baz"));
  let tree = RT.render(<TestFormComponent initialValues=initial />);
  let btn = tree |> RT.getByText(~matcher=`Str("VALIDATE"));
  let options = DT.WaitForElement.makeOptions(~container=btn, ());
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
  RT.FireEvent.click(btn);
});