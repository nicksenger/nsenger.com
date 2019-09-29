type formState('a) = {
  touched: Belt.Set.String.t,
  values: Js.Dict.t(string),
  errors: Js.Dict.t('a),
};

let getFormStateReducer =
    (
      validator,
      validateOnChange,
      validateOnBlur,
      state: formState('a),
      action: Types.formAction,
    ) =>
  switch (action) {
  | Types.Blur(name) =>
    let touched = Belt.Set.String.add(state.touched, name);
    let errors = validateOnBlur ? validator(state.values) : state.errors;
    let newState = {touched, values: state.values, errors};
    newState;
  | Types.Change(name, value) =>
    let touched = Belt.Set.String.add(state.touched, name);
    Js.Dict.set(state.values, name, value);
    let errors = validateOnChange ? validator(state.values) : state.errors;
    let newState = {touched, values: state.values, errors};
    newState;
  | Types.Validate =>
    let errors = validator(state.values);
    let newState = {touched: state.touched, values: state.values, errors};
    newState;
  };

let useForm =
    (
      ~validateOnChange=true,
      ~validateOnBlur=true,
      initialValues: Js.Dict.t(string),
      validator: Js.Dict.t(string) => Js.Dict.t('a),
    ) => {
  let reducer =
    getFormStateReducer(validator, validateOnChange, validateOnBlur);
  let initialState: formState('a) = {
    touched: Belt.Set.String.empty,
    values: initialValues,
    errors: Js.Dict.empty(),
  };
  let (state, dispatch) = React.useReducer(reducer, initialState);
  let handleBlur = e =>
    dispatch(Types.Blur(ReactEvent.Focus.target(e)##name));
  let handleChange = e =>
    dispatch(
      Types.Change(
        ReactEvent.Form.target(e)##name,
        ReactEvent.Form.target(e)##value,
      ),
    );
  ();
  let validate = () => dispatch(Types.Validate);
  (state.values, state.errors, handleChange, handleBlur, validate);
};