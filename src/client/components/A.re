let getHref = (_url: ReasonReactRouter.url, href) => href;

[@react.component]
let make = (~href, ~className: string="", ~children) => {
  let url = UrlContext.useUrl();
  let href = getHref(url, href);
  <a
    href=href
    className=className
    onClick=(e => {
      ReactEvent.Mouse.preventDefault(e);
      ReasonReactRouter.push(href);
    })
  >
    (children)
  </a>
};