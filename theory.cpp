#include <Arduino.h>



float getNoteFreq(int n) {
  return 440.0 * pow(2, (n) / 12.0);
}

int modifyNote(int note, bool swapMajMin, bool diminish, bool swapSeventh, bool sus4) {
    if (swapMajMin) {
      if (note == 4) {
        note = 3; // make major third minor
      } else if (note == 3) {
        note = 4; // make minor third major
      }
      // do the same for higher thirds
      if (note == 4+12) {
        note = 3+12;
      } else if (note == 3+12) {
        note = 4+12;
      }
      if (note == 4+24) {
        note = 3+24;
      } else if (note == 3+24) {
        note = 4+24;
      }
    }
    if (diminish) {
      if (note == 0) {
        note = 1; // move root up a half step to diminish the chord
      } if (note == 12) {
        note = 1+12; // move root up a half step to diminish the chord
      } if (note == 24) {
        note = 1+24; // move root up a half step to diminish the chord
      }
    }
    if (swapSeventh) {
      if (note == 11) {
        note = 10; // flatten natural sevenths
      } else if (note == 10) {
        note = 11; // sharpen flat sevenths
      }
      if (note == 11+12) {
        note = 10+12; // flatten natural sevenths
      } else if (note == 10+12) {
        note = 11+12; // sharpen flat sevenths
      }
      if (note == 11+24) {
        note = 10+24; // flatten natural sevenths
      } else if (note == 10+24) {
        note = 11+24; // sharpen flat sevenths
      }
    }
    if (sus4) {
      if (note == 4) {
        note = 5;
      }
      if (note == 4+12) {
        note = 5+12;
      }
      if (note == 4+24) {
        note = 5+24;
      }
    }
  return note;
}