#include "pattern.hpp"


int scaleSteps[] = {0, 2, 4, 5, 7, 9, 11, 12};

// Each row has the notes of the major scale chords.
// Each number is the number of half steps above the chord root the note is.
int chords[8][8] = {
  // first octave   2nd octave
  {0, 4, 7, 11}, // I (major)
  {0, 3, 7, 10}, // ii (minor)
  {0, 3, 7, 10}, // iii (minor)
  {0, 4, 7, 11}, // IV (major)
  {0, 4, 7, 10}, // V (dominant)
  {0, 3, 7, 10}, // vi (minor)
  {0, 3, 6, 10}, // vii (diminished)
  {0, 4, 7, 11}  // I (major)
};


// Each pattern specifies the number of notes and which indices of the chords[] subarrays to play.
// For example, {{0, 1, 2, 3}, 4} is a four-note pattern that plays all four notes from lowest to highest.
Pattern ascending = {4, 999, { 0, 1, 2, 3 }}; // ascending
Pattern descending = {4, 999,{ 0, 1, 2, 3 }}; // descending
Pattern triplet = {3, 999, { 0, 1, 2}}; // triplet
Pattern alberti = {4, 999, { 0, 2, 1, 2 }}; // alberti
Pattern bossa = {8, 999, { 0, 0, 0, 3, 1, 1, 2, 2 }}; // bossa

Pattern* patternPtrs[] = {&ascending, &descending, &triplet, &alberti, &bossa};


float getNoteFreq(int n);

