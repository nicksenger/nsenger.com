let app = Express.App.make();
Express.App.use(app, BodyParser.json());
let renderHTML = (_next, req, res) => {
  let url: ReasonReactRouter.url = {
    path:
      List.filter(
        s => s != "",
        Array.to_list(Js.String.split("/", Express.Request.path(req))),
      ),
    hash: "",
    search: "",
  };
  let content =
    ReactDOMServerRe.renderToString(<Body serverUrl=(Some(url)) />);
  Express.Response.sendString(
    {j|
      <!DOCTYPE html>
      <html>
        <head>
          <meta charset="utf-8">
          <meta name="viewport" content="width=device-width, initial-scale=1">
          <title>Senger.io</title>
          <link rel="stylesheet" href="/static/bundle.css">
          <meta name="theme-color" content="#ffffff">
        </head>
        <body class="sio__body">
          <div id="root" class="sio__container">
            $content
          </div>
        </body>
        <script defer>
          var spa = document.createElement('script');
          spa.type = "text/javascript";
          spa.src = "/static/bundle.js";
          document.body.appendChild(spa);
        </script>
      </html>
    |j},
    res,
  );
};

Express.Static.defaultOptions()
|> Express.Static.make("src/client/dist/")
|> Express.Static.asMiddleware
|> Express.App.useOnPath(app, ~path="/static");

Endpoints.sendMessage
  |> Express.PromiseMiddleware.from
  |> Express.App.useOnPath(~path="/send-message", app);

renderHTML |> Express.Middleware.from |> Express.App.useOnPath(~path="/", app);

let port = 3000;
let onListen = e =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log("listening at localhost:" ++ string_of_int(port))
  };

Express.App.listen(app, ~onListen, ~port, ());