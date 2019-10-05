let validator = values => {
  let d = Js.Dict.empty();

  switch (Js.Dict.get(values, "input1")) {
  | Some(s) =>
    switch (s) {
    | "foo" => Js.Dict.set(d, "input1", "input1 can NEVER be 'foo'")
    | _ => ignore()
    }
  | _ => Js.Dict.set(d, "input1", "input1 is required")
  };

  switch (Js.Dict.get(values, "input2")) {
  | Some(s) =>
    switch (s) {
    | "bar" => Js.Dict.set(d, "input2", "input2 can NEVER be 'bar'")
    | _ => ignore()
    }
  | _ => Js.Dict.set(d, "input2", "input2 is required")
  };

  switch (Js.Dict.get(values, "input3")) {
  | Some(s) =>
    switch (s) {
    | "baz" => Js.Dict.set(d, "input3", "input3 can NEVER be 'baz'")
    | _ => ignore()
    }
  | _ => Js.Dict.set(d, "input3", "input3 is required")
  };

  d;
};

let valueOrEmpty = (values, key) =>
  switch (Js.Dict.get(values, key)) {
  | Some(s) => s
  | None => ""
  };

let any = o => List.fold_right((||), o, false);

let hasErrors = (errors, keys) =>
  List.map(
    k =>
      switch (Js.Dict.get(errors, k)) {
      | Some(_s) => true
      | None => false
      },
    keys,
  )
  |> any;

[@react.component]
let make =
    (
      ~validateOnChange=true,
      ~validateOnBlur=true,
      ~initialValues=Js.Dict.empty(),
    ) => {
  let (values, errors, handleChange, handleBlur, validate) =
    Hooks.useForm(
      initialValues,
      validator,
      ~validateOnChange,
      ~validateOnBlur,
    );
  ();

  <>
    <label>
      (React.string("INPUT 1"))
      <input
        name="input1"
        value=(valueOrEmpty(values, "input1"))
        onChange=handleChange
        onBlur=handleBlur
      />
    </label>
    <label>
      (React.string("INPUT 2"))
      <input
        name="input2"
        value=(valueOrEmpty(values, "input2"))
        onChange=handleChange
        onBlur=handleBlur
      />
    </label>
    <label>
      (React.string("INPUT 3"))
      <input
        name="input3"
        value=(valueOrEmpty(values, "input3"))
        onChange=handleChange
        onBlur=handleBlur
      />
    </label>
    (switch (hasErrors(errors, ["input1", "input2", "input3"])) {
    | true => <button disabled=true>(React.string("SUBMIT"))</button>
    | false => <button>(React.string("SUBMIT"))</button>
    })
    <button onClick=((_) => validate())>(React.string("VALIDATE"))</button>
  </>;
};