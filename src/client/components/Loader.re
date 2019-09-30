[@react.component]
let make = (~visible) =>
  <div className=(visible ? "sio__loader" : "sio__loader sio__loader--hidden")>
    <div className="sio__spinner">
      <YiLoader size=(50) />
    </div>
  </div>