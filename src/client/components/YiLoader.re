[@react.component]
let make = (~size: int) => {
  let (hexagram, changeHexagram) = React.useState(_ => IChing.divine());
  React.useEffect(() => {
    let timerId = Js.Global.setInterval(() => changeHexagram(_ => IChing.divine()), 200);
    Some(() => Js.Global.clearInterval(timerId));
  });
  <IChing hexagram=hexagram size=size />
}