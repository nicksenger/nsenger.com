type line =
  | Yin
  | Yang;
type hexagram = list(line);

let flatMap = (f, xs) => xs |> List.map(f) |> List.flatten;
let addYinYang = l => [[Yin, ...l], [Yang, ...l]];
let binaryToKingWen = [
  2,
  23,
  8,
  20,
  16,
  35,
  45,
  12,
  15,
  52,
  39,
  53,
  62,
  56,
  31,
  33,
  7,
  4,
  29,
  59,
  40,
  64,
  47,
  6,
  46,
  18,
  48,
  57,
  32,
  50,
  28,
  44,
  24,
  27,
  3,
  42,
  51,
  21,
  17,
  25,
  36,
  22,
  63,
  37,
  55,
  30,
  49,
  13,
  19,
  41,
  60,
  61,
  54,
  38,
  58,
  10,
  11,
  26,
  5,
  9,
  34,
  14,
  43,
  1,
];

/* Generate the 64 hexagrams of the I Ching using binary method,
   then sort according to the King Wen sequence */
let hexagrams =
  []
  |> addYinYang
  |> flatMap(addYinYang)
  |> flatMap(addYinYang)
  |> flatMap(addYinYang)
  |> flatMap(addYinYang)
  |> flatMap(addYinYang)
  |> Belt.List.zip(binaryToKingWen)
  |> List.sort((a, b) => fst(a) - fst(b))
  |> List.map(snd);

[@react.component]
let make = () => {
  let size = 45;
  ReasonReact.array(
    Array.of_list(
      List.mapi(
        (i1, h) =>
          <div
            key=(string_of_int(i1))
            className="iching__container"
            style=(
              ReactDOMRe.Style.make(
                ~width=string_of_int(size) ++ "px",
                ~height=string_of_int(size - size / 5) ++ "px",
                (),
              )
            )>
            <svg viewBox="0 0 15 12" className="iching__svg">
              (
                ReasonReact.array(
                  Array.of_list(
                    List.mapi(
                      (i2, l) => {
                        let offset = string_of_int(13 - 2 * abs(6 - i2));
                        switch (l) {
                        | Yin =>
                          <g key=(string_of_int(i2))>
                            <path d={j|M1,$offset L5,$offset|j} />
                            <path d={j|M10,$offset L14,$offset|j} />
                          </g>
                        | Yang =>
                          <g key=(string_of_int(i2))>
                            <path d={j|M1,$offset L7,$offset|j} />
                            <path d={j|M8,$offset L14,$offset|j} />
                          </g>
                        };
                      },
                      h,
                    ),
                  ),
                )
              )
            </svg>
          </div>,
        hexagrams,
      ),
    ),
  );
};