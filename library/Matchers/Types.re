/*
 * Types.re
 */

/*--------------------------- Default Matcher ------------------------------*/

module IndexMatchResult = {
  type t = {
    score: int,
    indicies: array(int),
  };

  let create = (score: int, indicies: array(int)) => {score, indicies};
};

module MatchResult = {
  type t = {score: int};

  let create = (score: int) => {score: score};
};

let awfulScore = (-4000000000000000000);

module Action = {
  type t =
    | Miss
    | Match;
};

module CharType = {
  type t =
    | NoChar
    | Upper
    | Lower
    | Separ
    | Other;

  let charTypeOf = (char: option(Char.t)) => {
    switch (char) {
    | None => NoChar
    | Some(charT) =>
      switch (charT) {
      | 'A'..'Z' => Upper
      | 'a'..'z' => Lower
      | ' '
      | '_'
      | '-'
      | '/'
      | '\\' => Separ
      | _ => Other
      }
    };
  };
};

module CharRole = {
  type t =
    | Tail
    | Head;

  let charRole = (prev: option(Char.t), current: option(Char.t)) => {
    switch (CharType.charTypeOf(prev), CharType.charTypeOf(current)) {
    | (CharType.NoChar, CharType.Other)
    | (CharType.NoChar, CharType.Upper)
    | (CharType.Other, CharType.Upper)
    | (CharType.Separ, CharType.Other)
    | (CharType.Separ, CharType.Upper) => Head
    | _ => Tail
    };
  };
};

module Score = {
  type t = {
    lastActionMiss: ref(Action.t),
    lastActionMatch: ref(Action.t),
    missScore: ref(int),
    matchScore: ref(int),
  };

  let default: t = {
    lastActionMiss: ref(Action.Miss),
    lastActionMatch: ref(Action.Miss),
    missScore: ref(awfulScore),
    matchScore: ref(awfulScore),
  };

  let getDefault = (_: int) => {
    default;
  };

  let arrayOfDefault = (_: int, ~lineLen: int) => {
    let array = Array.init(lineLen + 1, getDefault);

    array;
  };

  let matrixOfDefault = (maxRows, lineLen) => {
    let matrix = Array.init(maxRows, arrayOfDefault(~lineLen));

    matrix;
  };
};

/*------------------------------ Path Matcher ------------------------------*/

module PathScore = {
  type t = {
    bonusUpperMatch: int,
    bonusAdjacency: int,
    bonusSeparator: int,
    bonusCamel: int,
    penaltyCaseUnmatched: int,
    penaltyLeading: int,
    penaltyMaxLeading: int,
    penaltyUnmatched: int,
  };

  let default: t = {
    bonusUpperMatch: 10,
    bonusAdjacency: 10,
    bonusSeparator: 8,
    bonusCamel: 8,
    penaltyCaseUnmatched: (-1),
    penaltyLeading: (-6),
    penaltyMaxLeading: (-18),
    penaltyUnmatched: (-2),
  };
};

module MatchingStatus = {
  type t = {
    index: int,
    score: int,
    finalScore: int,
    adjNum: int,
    backRef: int,
  };

  let default: t = {index: 0, score: 0, finalScore: 0, adjNum: 1, backRef: 0};
};