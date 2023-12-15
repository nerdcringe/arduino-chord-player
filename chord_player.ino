#include <Arduino.h>
#include "myEncoder.hpp"
#include "myLCD.hpp"
#include "keyRegister.hpp"
#include "theory.hpp"


#define PATTERN_DELAY_POT A7
/*#define MAJ_MINOR_SW 1
#define DIM_SWITCH 2*/


int wrapIndex(int i, int max) {
  return ((i % max) + max) % max;
}


#define KEY_PAGE 0
#define OCTAVE_PAGE 1
#define PATTERN_PAGE 2
#define VOICE_PAGE 3
#define NUM_PAGES 4
int pageIndex = 0;
Page pages[] {
  { "Key", 12, 0, { "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab" } },
  { "Octave", 6, 2, { "1", "2", "3", "4", "5", "6"} },
  { "Pattern", 7, 0, {"1 - Ascending", "2 - Descending", "3 - Triplet", "4 - Single", "5 - Octave", "6 - Alberti", "7 - Bossa"}},
  { "Voice", 3, 1, {"Square", "Sine", "Triangle"} },
};

// records the number of note within the current pattern
int currentPlaceInPattern = 0;
unsigned long lastNoteChangeTime = 0; // track last time the note within the pattern changed

// when holding down multiple keys, it plays the pattern for each key sequentially
int currentChordInSeq = 0;


int lastEncSwState = 0;
int lastEncPosition = 0;
void menuControl(Page** currentPage, bool* pageChanged, bool* selectionChanged) {
  int encSwState = getEncoderPressed();
  int encPosition = getEncoderPosition();

  // check when encoder switch is first pressed (went from HIGH to LOW)
  if (encSwState == LOW && lastEncSwState == HIGH) {
    pageIndex++;  // go to next page
    pageIndex = wrapIndex(pageIndex, NUM_PAGES); // wrap index within page array size
    *currentPage = &pages[pageIndex];
    *pageChanged = true;
  }

  // change selection on current page
  int encPosChange = encPosition - lastEncPosition;
  if (encPosChange > 0) {
    (*currentPage)->selectedIndex++;
    *selectionChanged = true;
  } else if (encPosChange < 0) {
    (*currentPage)->selectedIndex--;
    *selectionChanged = true;
  }

  lastEncSwState = encSwState;
  lastEncPosition = encPosition;
}


void menuRefresh(Page page, bool pageChanged) {
  if (pageChanged) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(page.title);
  }
  lcd.setCursor(0, 1);
  lcd.print(getOption(page, page.selectedIndex) + "    ");
}



void setup() {
  Serial.begin(9600);
  pinMode(PATTERN_DELAY_POT, INPUT_PULLUP);
  initEncoder();
  initKeyRegister();
  lcd.begin(16, 2);
  menuRefresh(pages[pageIndex], true);
}


void loop() {
  Page* currentPage = &pages[pageIndex];
  bool pageChanged = false;
  bool selectionChanged = false;
  menuControl(&currentPage, &pageChanged, &selectionChanged);
  
  if (pageChanged || selectionChanged) {
    // wrap around selected index to keep within array range
    currentPage->selectedIndex = wrapIndex(currentPage->selectedIndex, currentPage->numOptions);
    menuRefresh(*currentPage, pageChanged);
  }


  byte incoming = keyShiftIn();

  // count number of keys held
  int numKeysHeld = 0;
  for (int i = 0; i < 8; i++) {
    if (!(incoming & (1 << i))) {
      numKeysHeld++;
    }
  }

  if (numKeysHeld > 0) {
    Pattern* currentPattern = patternPtrs[pages[PATTERN_PAGE].selectedIndex];
    // Set the pattern delay based on potentiometer value
    int patternDelay = 250 * (analogRead(PATTERN_DELAY_POT) / 1024.0);

    if (millis() - lastNoteChangeTime > patternDelay) {
      currentPlaceInPattern++;

        Serial.print(millis());
        Serial.print("   ");
        Serial.print(lastNoteChangeTime);
        Serial.print("   ");
        Serial.println(patternDelay);


      if (currentPlaceInPattern >= currentPattern->numNotes) {
        currentPlaceInPattern = 0;
        currentChordInSeq++;
      }
      lastNoteChangeTime = millis();
      
      if (currentChordInSeq >= numKeysHeld) {
        currentChordInSeq = 0;
      }
    }

    int currentKeyRootNum = 0;
    int numKeysAccounted = 0;
    for (int i = 0; i < 8; i++) {
      if (!(incoming & (1 << i))) {
        numKeysAccounted++;
        if (numKeysAccounted == currentChordInSeq + 1) {
          currentKeyRootNum = i;
          break;
        }
      }
    }
    
    // select which note index of the chord to play
    int chordNoteIndex = currentPattern->chordNoteIndices[currentPlaceInPattern];
    // find out which note to play by accessing the chord note at the given index
    int stepsAboveRoot = chords[currentKeyRootNum][chordNoteIndex];
    // Obtain the current key and octave from their respective settings pages
    int keyOffset = pages[KEY_PAGE].selectedIndex;
    int octaveOffSet = ((pages[OCTAVE_PAGE].selectedIndex-3) * 12);

    // find out which note number on the piano to play (1-88)
    int noteNum = scaleSteps[currentKeyRootNum] + stepsAboveRoot + keyOffset + octaveOffSet;
    int noteFreq = getNoteFreq(noteNum); // calculate the frequency in hertz
    tone(SPKR, noteFreq); // play tone through speaker

  } else {
    currentPlaceInPattern = 0;
    currentChordInSeq = 0;
    lastNoteChangeTime = millis();
    noTone(SPKR);
  }

  delay(5);
}