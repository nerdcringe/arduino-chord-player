#include <Arduino.h>
#include "myEncoder.hpp"
#include "outputRegister.hpp"
#include "inputRegister.hpp"
#include "signalGen.hpp"
#include "page.hpp"
#include "theory.hpp"


#define TEMPO_POT A7
#define VIBRATO_POT A6
#define SPKR_VOLTAGE_CTRL A4
#define SIGNAL_READ A3


int wrapIndex(int i, int max) {
  return ((i % max) + max) % max;
}


#define PATTERN_PAGE 0
#define VOICE_PAGE 1
#define OCTAVE_PAGE 2
#define KEY_PAGE 3
#define NUM_PAGES 4
int pageIndex = 0;
Page pages[] {
  { "Pattern", 14, 5, { "Single", "Octave", "Triad (Root)", "Triad (1st Inv)", "Triad (2nd Inv)", "Ascending", "Descending",
    "Alberti", "Bossa", "Reggaeton", "Ninths", "Doo-Wop", "Strum", "Random"} },
  { "Voice", 3, 0, {"Sine", "Triangle", "Square "} },
  { "Octave", 6, 3, { "1", "2", "3", "4", "5", "6"} },
  { "Key", 12, 0, { "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab" } },
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
  lcd.setCursor(16-5 - (page.numOptions/10) - ((page.selectedIndex+1)/10), 0);
  lcd.print("  ");
  lcd.print(page.selectedIndex+1);
  lcd.print("/");
  lcd.print(page.numOptions);
  lcd.setCursor(0, 1);
  lcd.print(getOption(page, page.selectedIndex) + "       ");
}



void setup() {
  Serial.begin(9600);
  pinMode(TEMPO_POT, INPUT_PULLUP);
  pinMode(VIBRATO_POT, INPUT_PULLUP);
  pinMode(SPKR_VOLTAGE_CTRL, OUTPUT);
  pinMode(SIGNAL_READ, INPUT);
  initEncoder();
  initKeyRegister();
  initSignalGen();
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
    //currentPage->selectedIndex = wrapIndex(currentPage->selectedIndex, currentPage->numOptions);
    currentPage->selectedIndex = max(currentPage->selectedIndex, 0);
    currentPage->selectedIndex = min(currentPage->selectedIndex, currentPage->numOptions-1);
    menuRefresh(*currentPage, pageChanged);
  }

  
  if (pages[VOICE_PAGE].selectedIndex == 0) {
    setWaveform(AD9833_SINE);
    digitalWrite(SPKR_VOLTAGE_CTRL, HIGH);
  } else if (pages[VOICE_PAGE].selectedIndex == 1) {
    setWaveform(AD9833_TRIANGLE);
    digitalWrite(SPKR_VOLTAGE_CTRL, HIGH);
  } else if (pages[VOICE_PAGE].selectedIndex == 2) {
    setWaveform(AD9833_SQUARE1);
    digitalWrite(SPKR_VOLTAGE_CTRL, LOW);
  }



  int incoming = keyShiftIn();
  // count number of keys held
  int numKeysHeld = 0;
  for (int i = 0; i < 8; i++) {
    if (!(incoming & 1<<i)) {
      numKeysHeld++;
    }
  }

  if (numKeysHeld > 0) {
    Pattern* currentPattern = patternPtrs[pages[PATTERN_PAGE].selectedIndex];
    // Set the pattern delay based on potentiometer value
    float normTempo = map(analogRead(TEMPO_POT), 0, 1023, 0, 10000);
    normTempo /= 10000;
    normTempo = normTempo*normTempo; // square the normalized value to make smaller values (faster tempos) closer together
    normTempo *= 10000;
    int tempo = map(normTempo, 0, 10000, 15, 750) * currentPattern->tempoMultiplier;
    //int tempo = map(analogRead(TEMPO_POT), 0, 1023, 3, 500);//400 * (analogRead(TEMPO_POT) / 1023.0);

    if (millis() - lastNoteChangeTime > tempo) {
      currentPlaceInPattern++;

      lastRandomNote = (int)random(0, 11);
      if (lastRandomNote > 7) lastRandomNote = 0;

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
    
    // get the number of diatonic steps above the chord root
    int diatonicStepsAboveRoot = currentPattern->diatonicSteps[currentPlaceInPattern];
    
    if (patternPtrs[pages[PATTERN_PAGE].selectedIndex] == &randomNotes) {
      diatonicStepsAboveRoot = lastRandomNote;
    }

    if (diatonicStepsAboveRoot != -1) {
      // get the number of absolute steps above the tonic note of the scale
      int stepsAboveTonic = scaleSteps[diatonicStepsAboveRoot + currentKeyRootNum];

      // test for 0s on the second PISO shift register to apply modifiers
      bool swapMajMin = !(incoming & 1<<8);
      bool diminish = !(incoming & 1<<9);
      bool swapSeventh = false;//!(incoming & 1<<10);
      bool sus4 = !(incoming & 1<<10);
      
      float vibratoOffset = 0;
      if (!(incoming & 1<<11)) { // add vibrato through button
        vibratoOffset = 10*sin(((double)millis())*0.05);
      }
      
      int rootStepsAboveTonic = scaleSteps[currentKeyRootNum]; // subtract this so chord can be modified the same regardless of scale step
      stepsAboveTonic = modifyNote(stepsAboveTonic - rootStepsAboveTonic, swapMajMin, diminish, swapSeventh, sus4) + rootStepsAboveTonic;
      
      // Obtain the current key and octave from their respective settings pages
      int keyOffset = pages[KEY_PAGE].selectedIndex;
      int octaveOffSet = ((pages[OCTAVE_PAGE].selectedIndex-3) * 12);
      // find out which note number on the piano to play (1-88)
      int noteNum = stepsAboveTonic + keyOffset + octaveOffSet;
      int noteFreq = getNoteFreq(noteNum) + vibratoOffset; // calculate the frequency in hertz
      //tone(SPKR, noteFreq); // play tone through speaker
      
      setSignalFrequency(noteFreq);
    } else {
      //noTone(SPKR);
      stopSignal();
    }

  } else {
    currentPlaceInPattern = 0;
    currentChordInSeq = 0;
    lastNoteChangeTime = millis();
    //noTone(SPKR);
    stopSignal();
  }

  Serial.print("Signal:");
  Serial.print(analogRead(A3));
  Serial.println(",");

  delay(5);
}