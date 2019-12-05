[@react.component]
let make = (~visible: bool) =>
  <section
    className=(visible ? "sio__about" : "sio__about sio__about--hidden")>
    <p>
      (
        React.string(
          {j|I am a software engineer with several years of experience building scalable
             web & native applications using modern techniques. If you need technical
             assistance developing an app or project, feel free to reach out |j},
        )
      )
      <span className="sio__small-only"> (React.string(".")) </span>
      <span className="sio__big-only">
        (React.string({j| using the form below:|j}))
      </span>
    </p>
  </section>;