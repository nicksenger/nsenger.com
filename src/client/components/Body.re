let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Types.Home
  | ["home"] => Types.Home
  | ["about"] => Types.About
  | ["contact"] => Types.Contact
  | _ => Types.Home
  };

type state = {menuOpen: bool};

type action =
  | ToggleMenu;

[@react.component]
let make = (~serverUrl: option(ReasonReactRouter.url)) => {
  let (menuOpen, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ToggleMenu =>
          switch (state) {
          | Types.Initial => Types.Open
          | Types.Open => Types.Closed
          | Types.Closed => Types.Open
          }
        },
      Types.Initial,
    );
  let toggleMenu = () => dispatch(ToggleMenu);
  let url =
    switch (serverUrl) {
    | Some(url) => url
    | None => ReasonReactRouter.useUrl()
    };
  let route = mapUrlToRoute(url);

  <>
    <UrlProvider value=url>
      <Header toggleMenu />
      <main className="sio__main">
        <Name visible=(route == Types.Home) />
        <About visible=(route == Types.About) />
        <Contact visible=(route == Types.Contact) />
      </main>
      <Burger menuOpen toggleMenu />
      <Footer route menuOpen />
    </UrlProvider>
  </>;
};