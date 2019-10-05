open Jest;
open Expect;

testAsync("getUrl path", finished => {
  let path = "foo/bar/baz/qux/corge/waldo/fred";
  let query = Js.Dict.empty();
  let expectedUrl: ReasonReactRouter.url = {
    path: ["foo", "bar", "baz", "qux", "corge", "waldo", "fred"],
    hash: "",
    search: "",
  };

  Server.getUrl(path, query) |> expect |> toEqual(expectedUrl) |> finished;
});

testAsync("getUrl contact success", finished => {
  let path = "contact/";
  let query = Js.Dict.empty();
  Js.Dict.set(query, "success", Js.Json.string("true"));
  let expectedUrl: ReasonReactRouter.url = {
    path: ["contact"],
    hash: "",
    search: "success=true",
  };

  Server.getUrl(path, query) |> expect |> toEqual(expectedUrl) |> finished;
});

testAsync("getUrl contact failure", finished => {
  let path = "contact/";
  let query = Js.Dict.empty();
  Js.Dict.set(query, "success", Js.Json.string("false"));
  let expectedUrl: ReasonReactRouter.url = {
    path: ["contact"],
    hash: "",
    search: "success=false",
  };

  Server.getUrl(path, query) |> expect |> toEqual(expectedUrl) |> finished;
});