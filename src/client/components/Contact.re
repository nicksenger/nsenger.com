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
  | None => true
  };

[@react.component]
let make = (~visible: bool, ~onSubmit, ~status) => {
  let (values, errors, handleChange, _, _) =
    Hooks.useForm(Js.Dict.empty(), validator);
  let email = Js.Dict.get(values, "email");
  let message = Js.Dict.get(values, "message");
  let emailError = Js.Dict.get(errors, "email");
  let messageError = Js.Dict.get(errors, "message");
  <section
    className=(visible ? "sio__contact" : "sio__contact sio__contact--hidden")>
    (
      switch (status) {
      | Types.Pending =>
        <>
          <form
            action="/send-message-no-js"
            method="post"
            onSubmit=(
              e => {
                ReactEvent.Form.preventDefault(e);
                switch (email) {
                | Some(email) =>
                  switch (message) {
                  | Some(message) => onSubmit(email, message)
                  | None => ()
                  }
                | None => ()
                };
              }
            )>
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
              disabled=(! isOk(emailError) || ! isOk(messageError))>
              (React.string("SUBMIT"))
            </button>
          </form>
        </>
      | Types.Success =>
        <>
          <svg
            xmlns="http://www.w3.org/2000/svg"
            width="32"
            height="32"
            viewBox="0 0 32 32"
            className="sio__checkmark">
            <path
              fill="currentColor"
              d="M11.927 22l-6.882-6.883-3 3L11.927 28 31.204 8.728l-3.001-3.001z"
            />
          </svg>
          <p>
            (
              React.string(
                "Thank you for your submission. I'll be in touch as soon as possible.",
              )
            )
          </p>
        </>
      | _ =>
        <>
          <svg
            xmlns="http://www.w3.org/2000/svg"
            width="32"
            height="32"
            viewBox="0 0 32 32"
            className="sio__xmark">
            <path
              fill="currentColor"
              d="M18.404 16l9.9 9.9-2.404 2.404-9.9-9.9-9.9 9.9L3.696 25.9l9.9-9.9-9.9-9.898L6.1 3.698l9.9 9.899 9.9-9.9 2.404 2.406-9.9 9.898z"
            />
          </svg>
          <p>
            (
              React.string(
                "Oops, there was an issue sending your message. Please try again later.",
              )
            )
          </p>
        </>
      }
    )
  </section>;
};