type cache('a) = {
  addAll: list(string) => Js.Promise.t('a),
  match: unit => Js.Promise.t(Js.Promise.t(unit)),
};
type event = {
  waitUntil: Js.Promise.t(unit) => unit,
  request: unit,
  respondWith: Js.Promise.t(unit) => unit,
};

[@bs.scope "self"] [@bs.val]
external addEventListener : (string, event => unit) => unit =
  "addEventListener";
[@bs.scope "caches"] [@bs.val]
external open_ : string => Js.Promise.t(cache(unit)) = "open";
let cacheName = "senger-io-v1";
let filesToCache = [
  "/static/prof.jpeg",
  "/static/bundle.css",
  "/static/bundle.js",
  "index.html",
];

addEventListener("install", _e => {
  let waitUntil: Js.Promise.t('a) => unit = [%bs.raw {| _e.waitUntil |}];
  waitUntil(
    open_(cacheName)
    |> Js.Promise.then_(_cache => {
         let addAll = [%bs.raw {| _cache.addAll(filesToCache) |}];
         addAll;
       }),
  );
});

addEventListener("fetch", _e => {
  let respondWith: 'a => 'b = [%bs.raw {| _e.respondWith |}];
  let p: Js.Promise.t('a) = [%bs.raw {| fetch(event.request) |}];
  respondWith(
    p
    |> Js.Promise.then_(response =>
         open_(cacheName)
         |> Js.Promise.then_(_cache => {
              ignore(
                [%bs.raw {| _cache.put(_e.request.url, response.clone()) |}],
              );
              response;
            })
         |> Js.Promise.catch(_err => {
              let match = [%bs.raw {| caches.match(e.request) |}];
              match;
            })
       ),
  );
});