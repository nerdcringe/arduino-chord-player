#include <Arduino.h>
#include <LiquidCrystal.h>
#include "encoder.hpp"
#include "page.hpp"
#include "theory.hpp"


#define BUZZER 3

#define BTN1 24
#define BTN2 28
#define BTN3 32
#define BTN4 36
#define BTN5 40
#define BTN6 44
#define BTN7 48
#define BTN8 52
#define NUM_BUTTONS 8

#define MAJ_MIN_SWITCH 8
#define DIM_SWITCH 10

#define DELAY_POT A4

int buttonPins[NUM_BUTTONS] = { BTN1, BTN2, BTN3, BTN4, BTN5, BTN6, BTN7, BTN8 };
int buttonStates[NUM_BUTTONS];
int buttonIndex = -1;

// only one note is played at a time, so chords are made of a sequence of notes (arpeggio)
int placeInPattern = 0; // the current index in the arpeggio sequence
unsigned long lastPatternStart; // last time patternIndex changed


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 51, en = 47, d4 = 43, d5 = 39, d6 = 35, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lastEncBtnState = HIGH;
int lastCounterVal = 0;


#define KEY_PAGE 0
#define OCTAVE_PAGE 1
#define INVERSION_PAGE 2
#define PATTERN_PAGE 3


const int numPages = 4;
int pageIndex = 0;
Page pages[] {
  { "Key", 12, 0, { "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab" } },
  { "Octave", 5, 2, { "1", "2", "3", "4", "5" } },
  { "Inversion", 3, 0, { "Root", "1st", "2nd" } },
  { "Pattern", 5, 0, { "Ascending", "Descending", "Triplet", "Alberti", "Bossa"} }
};

Page currentPage = pages[0];


int wrapIndex(int i, int max) {
  return ((i % max) + max) % max;
}


void setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN5, INPUT_PULLUP);
  pinMode(BTN6, INPUT_PULLUP);
  pinMode(BTN7, INPUT_PULLUP);
  pinMode(BTN8, INPUT_PULLUP);

  pinMode(MAJ_MIN_SWITCH, INPUT_PULLUP);
  pinMode(DIM_SWITCH, INPUT_PULLUP);

  pinMode(DELAY_POT, INPUT_PULLUP);

  initEncoder();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lastPatternStart = millis();
  Serial.begin(9600);


  for (int i = 0; i < 4; i++) {
    Serial.print(ascending.chordNoteIndices[i]);
  }
}


void loop() {
  // check when button is first pressed (went from HIGH to LOW)
  if (digitalRead(SW) == LOW && lastEncBtnState == HIGH) {
    pageIndex++;  // go to next page
    // wraparound index to keep within range
    pageIndex = wrapIndex(pageIndex, numPages);
  }
  lastEncBtnState = digitalRead(SW);

  // change selection on current page
  Page* currentPage = &pages[pageIndex];
  if (movedCW) {
    currentPage->selectedIndex++;
    movedCW = false;
  }
  if (movedCCW) {
    currentPage->selectedIndex--;
    movedCCW = false;
  }
  // wraparound selected index to keep within range
  currentPage->selectedIndex = wrapIndex(currentPage->selectedIndex, currentPage->numOptions);

  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  lcd.print(currentPage->title + "             ");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(getOption(*currentPage, currentPage->selectedIndex) + "             ");

  lastCounterVal = getCounter();

  bool anyPressed = false;
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int pin = buttonPins[i];
    int state = digitalRead(pin);
    if (state == LOW) {
      buttonIndex = i;
      anyPressed = true;
    }
  }
  if (!anyPressed) {
    buttonIndex = -1;
  }

  int delay = 250 * (analogRead(DELAY_POT) / 1024.0);

  int patternIndex = pages[PATTERN_PAGE].selectedIndex;
  int chordNoteIndex = patternPtrs[patternIndex]->chordNoteIndices[placeInPattern];

  if (buttonIndex != -1) {
    int stepsAboveRoot = chords[buttonIndex][chordNoteIndex];

    if (digitalRead(MAJ_MIN_SWITCH) == LOW) {
      // swap major and minor thirds
      if (stepsAboveRoot == 4) {
        stepsAboveRoot = 3; // make major third minor
      } else if (stepsAboveRoot == 3) {
        stepsAboveRoot = 4; // make minor third major
      }
    }

    if (digitalRead(DIM_SWITCH) == LOW) {
      if (stepsAboveRoot == 0) {
        stepsAboveRoot = 1; // move tonic up a half step to make diminished chord
      }
    }

    int keyOffset = pages[KEY_PAGE].selectedIndex;
    int octaveOffSet = ((pages[OCTAVE_PAGE].selectedIndex-3) * 12);
    int noteNum = scaleSteps[buttonIndex] + stepsAboveRoot + keyOffset + octaveOffSet;
    float noteFreq = getNoteFreq(noteNum);
    tone(BUZZER, noteFreq);

    if (millis() - lastPatternStart > delay) {
      placeInPattern++;
      if (placeInPattern >= patternPtrs[patternIndex]->numNotes) {
        placeInPattern = 0;
      }
      lastPatternStart = millis();
    }
  } else {
    placeInPattern = 0;
    noTone(BUZZER);
  }
}
