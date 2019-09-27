let defaultUrl: ReasonReactRouter.url = {
  path: ["home"],
  hash: "",
  search: ""
}
let context = React.createContext(defaultUrl);
let useUrl = () => React.useContext(context);