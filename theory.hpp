#include "pattern.hpp"


int scaleSteps[] = {0, 2, 4, 5, 7, 9, 11, 12};

// Each row has the notes of the major scale chords.
// Each number is the number of half steps above the chord root the note is.
// Currently, scale steps 1, 3, 5, 7, 8, 9, and 10 are defined
int chords[8][7] = {
  // first octave 2nd octave
  {0, 4, 7, 11,   12, 14, 16}, // I (major)
  {0, 3, 7, 10,   12, 14, 15}, // ii (minor)
  {0, 3, 7, 10,   12, 13, 15}, // iii (minor)
  {0, 4, 7, 11,   12, 14, 16}, // IV (major)
  {0, 4, 7, 10,   12, 14, 16}, // V (dominant)
  {0, 3, 7, 10,   12, 14, 15}, // vi (minor)
  {0, 3, 6, 10,   12, 13, 15}, // vii (diminished)
  {0, 4, 7, 11,   12, 14, 16}  // I (major)
};


#define ROOT 0
#define THIRD 1
#define DOMINANT 2
#define SEVENTH 3
#define HIGH_ROOT 4
#define NINTH 5
#define HIGH_THIRD 6


// Each pattern specifies the number of notes and which indices of the chords[] subarrays to play.
// For example, {{0, 1, 2, 3}, 4} is a four-note pattern that plays all four notes from lowest to highest.
Pattern ascending = {4, { ROOT, THIRD, DOMINANT, SEVENTH }};
Pattern descending = {4,{ SEVENTH, DOMINANT, THIRD, ROOT }};
Pattern triplet = {3, { ROOT, THIRD, DOMINANT }};
Pattern single = {1, { ROOT }};
Pattern octave = {2, { ROOT, HIGH_ROOT }};
Pattern alberti = {4, { ROOT, DOMINANT, THIRD, DOMINANT }};
Pattern bossa = {8, { ROOT, SEVENTH, HIGH_THIRD, ROOT, SEVENTH, HIGH_THIRD, HIGH_THIRD, ROOT }};
Pattern reggaton = {8, { ROOT, ROOT, ROOT, SEVENTH, THIRD, THIRD, DOMINANT, DOMINANT }};

Pattern* patternPtrs[] = {&ascending, &descending, &triplet, &single, &octave, &alberti, &bossa};


float getNoteFreq(int n);

