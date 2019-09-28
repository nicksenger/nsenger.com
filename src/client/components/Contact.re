[@react.component]
let make = (~visible: bool) =>
  <section
    className=(visible ? "sio__contact" : "sio__contact sio__contact--hidden")>
    <form>
      <label> (React.string("Return Email")) <input type_="email" /> </label>
      <label>
        (React.string("Message"))
        <textarea className="sio__message" />
      </label>
      <button className="sio__submit-btn" disabled=true>
        (React.string("SUBMIT"))
      </button>
    </form>
  </section>;