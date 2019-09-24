[@bs.config {jsx: 3}];

/* make the express app */
let app = Express.App.make();

/* Our initial rendering function, we will soon make this way better */
let renderHTML = (_next, _req, res) => {
  let content = ReactDOMServerRe.renderToString(<Component2 greeting="Hello!" />);
  Express.Response.sendString(content, res);
};

/* Express works on middleware and bs-express provides an easy way to make
   middleware out of functions */
renderHTML
|> Express.Middleware.from
|> Express.App.useOnPath(~path="/", app);

let port = 3000;

/* Getting a nice message when the server starts */
let onListen = e =>
  switch e {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log("listening at localhost:" ++ string_of_int(port))
  };

/* starting up the express app */
Express.App.listen(app, ~onListen, ~port, ());
