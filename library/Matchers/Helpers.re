/*
 * Helpers.re
 */

let reverseArray = (backwards: array('a)) => {
  let forwards: ref(array('a)) = ref([||]);

  for (i in Array.length(backwards) - 1 downto 0) {
    forwards := Array.append(forwards^, [|backwards[i]|]);
  };

  forwards^;
};

let splitArray = (inputArray: array('a), midPoint: int) => {
  let firstHalf = Array.sub(inputArray, 0, midPoint);
  let lastHalf =
    Array.sub(inputArray, midPoint, Array.length(inputArray) - midPoint);

  (firstHalf, lastHalf);
};

let compareEqualScore = (line1, line2) => {
  let score1Better = 1;
  let score2Better = (-1);
  let scoreEqual = 0;

  let finalScore = ref(scoreEqual);

  /* First, lets favour the shorter one */
  if (String.length(line1) != String.length(line2)) {
    if (String.length(line1) < String.length(line2)) {
      finalScore := score1Better;
    } else {
      finalScore := score2Better;
    };
  };

  /*
   * Is there anything else we should consider here?
   * For an index match, prefer compact ones?
   * For either, we could pass over counts of types of matches?
   *    Prefer camelcase etc.
   */

  finalScore^;
};

/* Compare two matches */
type indexfuzzyMatcher =
  (~line: string, ~pattern: string) => option(Types.IndexMatchResult.t);
type fuzzyMatcher =
  (~line: string, ~pattern: string) => option(Types.MatchResult.t);

/* Return the compare result on the two inputs */
let indexCompareInputs =
    (line1: string, line2: string, pattern: string, scorer: indexfuzzyMatcher) => {
  let scoreResult1 = scorer(~line=line1, ~pattern);
  let scoreResult2 = scorer(~line=line2, ~pattern);

  let (score1, score2) =
    switch (scoreResult1, scoreResult2) {
    | (Some(s1), Some(s2)) => (s1.score, s2.score)
    | (None, Some(s2)) => (Types.awfulScore, s2.score)
    | (Some(s1), None) => (s1.score, Types.awfulScore)
    | (_, _) => (Types.awfulScore, Types.awfulScore)
    };

  if (score1 != score2) {
    compare(score2, score1);
  } else {
    /*
     * Compare above will return 1 if score2 is greater than score1 and -1 for
     * the opposite. This will return 1 if score1 is better, so we need to
     * invert it.
     */
    compareEqualScore(line1, line2) * (-1);
  };
};

let compareInputs =
    (line1: string, line2: string, pattern: string, scorer: fuzzyMatcher) => {
  let scoreResult1 = scorer(~line=line1, ~pattern);
  let scoreResult2 = scorer(~line=line2, ~pattern);

  let (score1, score2) =
    switch (scoreResult1, scoreResult2) {
    | (Some(s1), Some(s2)) => (s1.score, s2.score)
    | (None, Some(s2)) => (Types.awfulScore, s2.score)
    | (Some(s1), None) => (s1.score, Types.awfulScore)
    | (_, _) => (Types.awfulScore, Types.awfulScore)
    };

  if (score1 != score2) {
    compare(score2, score1);
  } else {
    compareEqualScore(line1, line2) * (-1);
  };
};
