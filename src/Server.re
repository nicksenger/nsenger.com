let app = Express.App.make();
Express.App.use(app, BodyParser.json());
Express.App.use(app, BodyParser.urlencoded((), ~extended=true));

let getUrl = (path, query) => {
  let url: ReasonReactRouter.url = {
    path:
      List.filter(s => s != "", Array.to_list(Js.String.split("/", path))),
    hash: "",
    search:
      switch (Js.Dict.get(query, "success")) {
      | Some(v) =>
        switch (Js.Json.decodeString(v)) {
        | Some(s) =>
          switch (s) {
          | "true" => "success=true"
          | "false" => "success=false"
          | _ => ""
          }
        | _ => ""
        }
      | _ => ""
      },
  };
  url;
};

let renderHTML = (_next, req, res) => {
  let url = getUrl(Express.Request.path(req), Express.Request.query(req));
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
          <link rel="manifest" href="/static/webapp.manifest">
          <meta name="theme-color" content="#ffffff">
        </head>
        <body class="sio__body">
          <div id="root" class="sio__container">
            $content
          </div>
        </body>
        <script>
          if ('serviceWorker' in navigator) {
            window.addEventListener('load', function() {
              navigator.serviceWorker.register('/static/sw.js');
            });
          }
        </script>
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

Endpoints.sendMessageNoJs
|> Express.PromiseMiddleware.from
|> Express.App.useOnPath(~path="/send-message-no-js", app);

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
  | _ =>
    switch (Js.Dict.get(Node.Process.process##env, "MAILGUN_API_KEY")) {
    | Some(_) => Js.log("listening at localhost:" ++ string_of_int(port))
    | None => ()
    }
  };

Express.App.listen(app, ~onListen, ~port, ());