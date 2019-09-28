[@react.component]
let make = (~visible: bool) =>
  <section className=(visible ? "sio__name" : "sio__name sio__name--hidden")>
    <img src="static/prof.jpeg" className="sio__image" />
    <h3 className="sio__title"> (React.string("Nick Senger")) </h3>
  </section>;