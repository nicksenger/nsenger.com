[@react.component]
let make = (~visible: bool) =>
  <section className=(visible ? "sio__name" : "sio__name sio__name--hidden")>
    <h2 className="sio__title"> (React.string("Nick Senger")) </h2>
  </section>;