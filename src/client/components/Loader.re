[@react.component]
let make = (~visible) =>
  <div className=(visible ? "sio__loader" : "sio__loader--hidden")>
    <div className="sio__spinner">
      <IChing hexagram=(IChing.divine()) size=(50) />
    </div>
  </div>