let isValidEmail = s =>
  switch (s) {
  | Some(s) =>
    switch (Js.String.match([%re "/[^@]+@[^\\.]+\\..+/"], s)) {
    | None => false
    | _ => true
    }
  | None => true
  };
let isValidMessage = s =>
  switch (s) {
  | Some(s) => Js.String.length(s) > 0
  | None => true
  };

let validator = values => {
  let d = Js.Dict.empty();
  isValidEmail(Js.Dict.get(values, "email")) ?
    Js.Dict.set(d, "email", "ok") : Js.Dict.set(d, "email", "bad");
  isValidMessage(Js.Dict.get(values, "message")) ?
    Js.Dict.set(d, "message", "ok") : Js.Dict.set(d, "message", "bad");
  d;
};

let isOk = (s: option(string)) =>
  switch (s) {
  | Some(s) => s == "ok"
  | None => false
  };

[@react.component]
let make = (~visible: bool, ~onSubmit) => {
  let (values, errors, handleChange, _, _) =
    Hooks.useForm(Js.Dict.empty(), validator);
  let email = Js.Dict.get(values, "email");
  let message = Js.Dict.get(values, "message");
  let emailError = Js.Dict.get(errors, "email");
  let messageError = Js.Dict.get(errors, "message");
  <section
    className=(visible ? "sio__contact" : "sio__contact sio__contact--hidden")>
    <form>
      <label>
        (React.string("Return Email"))
        <input
          className=(
            switch (emailError) {
            | Some("bad") => "sio__email input-error"
            | _ => "sio__email"
            }
          )
          name="email"
          type_="email"
          onChange=handleChange
          value=(
            switch (email) {
            | Some(s) => s
            | None => ""
            }
          )
        />
      </label>
      <label>
        (React.string("Message"))
        <textarea
          className=(
            switch (messageError) {
            | Some("bad") => "sio__message input-error"
            | _ => "sio__message"
            }
          )
          name="message"
          onChange=handleChange
          value=(
            switch (message) {
            | Some(s) => s
            | None => ""
            }
          )
        />
      </label>
      <button
        className="sio__submit-btn"
        disabled=(! isOk(emailError) || ! isOk(messageError))
        onClick=(
          e => {
            ReactEvent.Mouse.preventDefault(e);
            switch (email) {
            | Some(email) =>
              switch (message) {
              | Some(message) => onSubmit(email, message)
              | None => ()
              }
            | None => ()
            }
          }
        )>
        (React.string("SUBMIT"))
      </button>
    </form>
  </section>;
};