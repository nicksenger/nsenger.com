let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Types.Home
  | ["home"] => Types.Home
  | ["about"] => Types.About
  | ["contact"] => Types.Contact
  | _ => Types.Home
  };

type menuState = {menuOpen: bool};
type menuAction =
  | ToggleMenu;
let menuReducer = (state, action) =>
  switch (action) {
  | ToggleMenu =>
    switch (state) {
    | Types.Initial => Types.Open
    | Types.Open => Types.Closed
    | Types.Closed => Types.Open
    }
  };

let submissionReducer:
  (Types.submissionState, Types.submissionAction) => Types.submissionState =
  (state, action) =>
    switch (action) {
    | Types.SubmitMessageRequest(_email, _message) => {
        loading: true,
        message: state.message,
        status: state.status,
      }
    | Types.SubmitMessageSuccess => {
        loading: false,
        message:
          Some(
            "Thank you for your submission, I'll respond as soon as possible!",
          ),
        status: Types.Success,
      }
    | Types.SubmitMessageFailure(e) => {
        loading: false,
        message: Some(e),
        status: Types.Failed,
      }
    };
let initialSubmissionState: Types.submissionState = {
  loading: false,
  message: None,
  status: Types.Pending,
};

[@react.component]
let make = (~serverUrl: option(ReasonReactRouter.url)) => {
  let (menuOpen, menuDispatch) =
    React.useReducer(menuReducer, Types.Initial);
  let (submissionState, submissionDispatch) =
    Hooks.useEpicReducer(
      submissionReducer,
      Epics.rootEpic,
      initialSubmissionState,
    );
  let submit = (e, m) =>
    submissionDispatch(Types.SubmitMessageRequest(e, m));
  let toggleMenu = () => menuDispatch(ToggleMenu);
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
        <Contact
          visible=(route == Types.Contact)
          onSubmit=submit
          status=submissionState.status
        />
      </main>
      <Burger menuOpen toggleMenu />
      <Footer route menuOpen />
    </UrlProvider>
    <Loader visible=submissionState.loading />
  </>;
};