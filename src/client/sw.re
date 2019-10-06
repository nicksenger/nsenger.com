[@bs.deriving abstract]
type request = {url: string};

[@bs.deriving abstract]
type event = {request};

type response;
type cache;

[@bs.val] external fetch : request => Js.Promise.t(response) = "fetch";
[@bs.scope "self"] [@bs.val]
external addEventListener : (string, event => unit) => unit =
  "addEventListener";
[@bs.scope "caches"] [@bs.val]
external open_ : string => Js.Promise.t(cache) = "open";
[@bs.scope "caches"] [@bs.val]
external match : request => Js.Promise.t(response) = "match";
[@bs.send]
external waitUntil : (event, Js.Promise.t('a)) => unit = "waitUntil";
[@bs.send]
external respondWith : (event, Js.Promise.t('a)) => unit = "respondWith";
[@bs.send]
external addAll : (cache, array(string)) => Js.Promise.t(unit) = "addAll";
[@bs.send] external put : (cache, string, response) => unit = "put";
[@bs.send] external clone : response => response = "clone";

let cacheName = "senger-io-v1";
let filesToCache = [
  "/",
  "/home",
  "/about",
  "/contact",
  "/index.html",
  "/static/prof.jpeg",
  "/static/bundle.css",
  "/static/bundle.js",
];

addEventListener("install", event =>
  waitUntil(
    event,
    open_(cacheName)
    |> Js.Promise.then_(cache => addAll(cache, Array.of_list(filesToCache))),
  )
);

let fetchWithFallback = event =>
  fetch(requestGet(event))
  |> Js.Promise.then_(response =>
       open_(cacheName)
       |> Js.Promise.then_(cache => {
            put(cache, urlGet(requestGet(event)), clone(response));
            Js.Promise.resolve(response);
          })
     )
  |> Js.Promise.catch(_err => match(requestGet(event)));

addEventListener("fetch", event =>
  respondWith(event, fetchWithFallback(event))
);