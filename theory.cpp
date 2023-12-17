#include <Arduino.h>

float getNoteFreq(int n) {
  return 440.0 * pow(2, (n) / 12.0);
}

int modifyNote(int note, bool swapMajMin, bool swapDiminish, bool swapSeventh) {
    if (swapMajMin) {
      if (note == 4) {
        note = 3; // make major third minor
      } else if (note == 3) {
        note = 4; // make minor third major
      }
      // do the same for high third
      if (note == 4+12) {
        note = 3+12;
      } else if (note == 3+12) {
        note = 4+12;
      }
    }
    if (swapDiminish) {
      if (note == 0) {
        note = 1; // move root up a half step to diminish the chord
      }
    }
    if (swapSeventh) {
      if (note == 11) {
        note = 10; // flatten natural sevenths
      } else if (note == 10) {
        note = 11; // sharpen flat sevenths
      }
    }
  return note;
}