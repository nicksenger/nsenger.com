[@react.component]
let make = (~visible: bool) =>
  <section
    className=(visible ? "sio__about" : "sio__about sio__about--hidden")>
    <p>
      (
        React.string(
          {j|I am a software engineer with experience building scalable web & native
             applications using modern techniques. Some things that interest me are:
             biotech, functional programming, foreign language study, and the I Ching.
             If you need technical assistance developing an app or project, feel free
             to reach out using the form below:|j},
        )
      )
    </p>
  </section>;