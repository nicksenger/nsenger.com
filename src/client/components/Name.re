[@react.component]
let make = (~visible: bool) =>
  <section className=(visible ? "sio__name" : "sio__name sio__name--hidden")>
    <h3> (React.string("Nick Senger")) </h3>
    <h4> (React.string("Developer")) </h4>
  </section>;