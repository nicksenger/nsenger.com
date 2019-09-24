type line = Yin | Yang;
type trigram = (line, line, line);
type hexagram = (trigram, trigram);

let trigrams: list(trigram) =
  [ ( Yin, Yin, Yin )
  , ( Yin, Yin, Yang )
  , ( Yin, Yang, Yin )
  , ( Yin, Yang, Yang )
  , ( Yang, Yang, Yang )
  , ( Yang, Yang, Yin )
  , ( Yang, Yin, Yang )
  , ( Yang, Yin, Yin )
  ];

let hexagrams: list(hexagram) = List.fold_right(
  (cur, acc) => List.append(acc, List.map(c => (cur, c), trigrams)),
  trigrams,
  []
);

[@react.component]
let make = (~hexagram: int, ~className: string, ~size: int, ~onClick) => {
  <div />
};
