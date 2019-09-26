type route =
  | Home
  | About
  | Contact;

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["home"] => Home
  | ["about"] => About
  | ["contact"] => Contact
  | _ => Home
  };

type state = {menuOpen: bool};

type action =
  | ToggleMenu;

[@react.component]
let make = (~serverUrl: option(ReasonReactRouter.url)) => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ToggleMenu => {menuOpen: ! state.menuOpen}
        },
      {menuOpen: false},
    );
  let url = switch (serverUrl) {
  | Some(url) => url
  | None => ReasonReactRouter.useUrl(())
  }
  let route = mapUrlToRoute(url);

  <>
    <Header />
    <main className="sio__main">
      <Name visible=(route == Home) />
      <About visible=(route == About) />
      <Contact visible=(route == Contact) />
    </main>
    <Footer />
  </>;
};