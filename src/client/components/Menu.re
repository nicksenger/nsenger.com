[@react.component]
let make = (~toggleMenu) => {
  let (hexagram, updateHexagram) = React.useState(() => IChing.divine());
  let changeHex = _ => updateHexagram(_ => IChing.divine());
  <div className="sio__menu-container" onClick=(_ => toggleMenu())>
    <div
      className="sio__menu-iching-small"
      onClick=(changeHex)
      onMouseOver=(changeHex)
      onMouseOut=(changeHex)>
      <IChing size=30 hexagram />
    </div>
  </div>;
};