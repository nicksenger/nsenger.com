open Jest;
open Expect;

testAsync("addition", finished => expect(1 + 2) |> toBe(3) |> finished);