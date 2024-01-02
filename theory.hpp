// all diatonic notes given in half steps relative to tonic
int scaleSteps[] = {0, 2, 4, 5, 7, 9, 11, 12+0, 12+2, 12+4, 12+5, 12+7, 12+9, 12+11, 24, 24+2, 24+4};

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



typedef struct pattern_struct {
  int numNotes;
  float tempoMultiplier;
  int diatonicSteps[];
} Pattern;


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
Pattern strum = {16, 0.5, { ROOT, THIRD, FIFTH, SEVENTH, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST, REST}};
Pattern randomNotes = {1, 1, {ROOT}};
int lastRandomNote = 0;

Pattern* patternPtrs[] = {&single, &octave, &triad_0, &triad_1, &triad_2, &ascending, &descending, &alberti, &bossa, &reggaeton, &ninths, &doowop, &strum, &randomNotes};


float getNoteFreq(int n);
int modifyNote(int stepsAboveRoot, bool swapMajMin, bool swapDiminish, bool swapSeventh, bool sus4);
