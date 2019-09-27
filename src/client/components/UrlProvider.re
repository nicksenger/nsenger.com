let provider = React.Context.provider(UrlContext.context);

[@react.component]
let make = (~value, ~children) =>
  React.createElement(provider, {"value": value, "children": children});