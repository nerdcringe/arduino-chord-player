// all diatonic notes given in half steps relative to tonic
int scaleSteps[] = {0, 2, 4, 5, 7, 9, 11, 12+0, 12+2, 12+4, 12+5, 12+7, 12+9, 12+11, 24, 24+2, 24+4};

// probably not going to implement alternate scales (just an idea)
/*int dorianSteps[] = {0, 2, 3, 5, 7, 9, 10, 12+0, 12+2, 12+3, 12+5, 12+7, 12+9, 12+10, 24, 24+2, 24+3};
int phrygianSteps[] = {0, 1, 3, 5, 7, 8, 10, 12+0, 12+1, 12+3, 12+5, 12+7, 12+8, 12+10, 24, 24+1, 24+3};
int lyadianSteps[] = {0, 2, 4, 6, 7, 9, 11, 12+0, 12+2, 12+4, 12+6, 12+7, 12+9, 12+11, 24, 24+2, 24+4};
int myxoLydianSteps[] = {0, 2, 4, 5, 7, 9, 10, 12+0, 12+2, 12+4, 12+5, 12+7, 12+9, 12+10, 24, 24+2, 24+4};
int minorSteps[] = {0, 2, 3, 5, 7, 8, 10, 12+0, 12+2, 12+3, 12+5, 12+7, 12+8, 12+10, 24, 24+2, 24+3};
int locrianSteps[] = {0, 1, 3, 5, 6, 8, 10, 12+0, 12+1, 12+3, 12+5, 12+6, 12+8, 12+10, 24, 24+1, 24+3};*/
 


typedef struct pattern_struct {
  int numNotes;
  float tempoMultiplier;
  int diatonicSteps[];
} Pattern;

// define diatonic steps in each chord.
// For example, ROOT is the root note, SECOND is 1 diatonic step above, THIRD is 2 above.
#define ROOT 0
#define SECOND 1
#define THIRD 2
#define FOURTH 3
#define FIFTH 4
#define SIXTH 5
#define SEVENTH 6
#define HIGH_ROOT 7
#define NINTH 8
#define HIGH_THIRD 9
#define REST -1


// Each pattern specifies the number of notes and which chord notes to play
Pattern single = {1, 1, { ROOT }};
Pattern octave = {2, 1, { ROOT, HIGH_ROOT }};
Pattern triad_0 = {3, 1, { ROOT, THIRD, FIFTH }};
Pattern triad_1 = {3, 1, {THIRD, FIFTH, HIGH_ROOT }};
Pattern triad_2 = {3, 1, { FIFTH, HIGH_ROOT, HIGH_THIRD }};
Pattern ascending = {8, 1, { ROOT, THIRD, FIFTH, SEVENTH, ROOT, THIRD, SEVENTH, SIXTH }};
Pattern descending = {8, 1, { SEVENTH, FIFTH, THIRD, ROOT, SEVENTH, FIFTH, ROOT, SECOND }};
Pattern alberti = {4, 1, { ROOT, FIFTH, THIRD, FIFTH }};
Pattern bossa = {8, 1, { ROOT, SEVENTH, HIGH_THIRD, ROOT, SEVENTH, HIGH_THIRD, HIGH_THIRD, ROOT }};
Pattern reggaeton = {8, 1, { ROOT, ROOT, ROOT, SEVENTH, THIRD, THIRD, FIFTH, FIFTH }};
Pattern ninths = {6, 1, { ROOT, THIRD, FIFTH, SEVENTH, NINTH, HIGH_THIRD }};
Pattern doowop = {8, 1, { ROOT, ROOT, REST, ROOT, HIGH_ROOT, HIGH_ROOT, REST, HIGH_ROOT}};
Pattern strum = {16, 0.25, { ROOT, THIRD, FIFTH, SEVENTH, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST}};
Pattern waltz = {12, 0.5, { ROOT, ROOT, ROOT, ROOT, THIRD, FIFTH, REST, REST, THIRD, FIFTH, REST, REST}};
Pattern randomNotes = {1, 1, {ROOT}};

Pattern* patternPtrs[] = {&single, &octave, &triad_0, &triad_1, &triad_2, &ascending, &descending, &alberti, &bossa, &reggaeton, &ninths, &doowop, &strum, &waltz, &randomNotes};
int lastRandomNote = 0;



float getNoteFreq(int n);
int modifyNote(int stepsAboveRoot, bool swapMajMin, bool swapDiminish, bool swapSeventh, bool sus4);
