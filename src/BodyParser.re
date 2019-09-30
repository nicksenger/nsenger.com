open Express;

type jsonOptions;
type urlencodedOptions;
[@bs.obj] external jsonOptions : (~inflate : bool=?) => (~strict : bool=?) => unit => jsonOptions = "";
[@bs.obj] external urlencodedOptions : (~extended : bool=?) => unit => urlencodedOptions = "";
[@bs.module "body-parser"] external json : jsonOptions => Middleware.t = "json";
[@bs.module "body-parser"] external urlencoded : urlencodedOptions => Middleware.t = "urlencoded";

let json = (~inflate=?, ~strict=?, ()) =>
    json(jsonOptions(~inflate?, ~strict?, ()));

let urlencoded = (~extended=?, ()) =>
    urlencoded(urlencodedOptions(~extended?, ()));