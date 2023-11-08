#include <Arduino.h>
#include <LiquidCrystal.h>
#include "encoder.hpp"
#include "page.hpp"


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
int currentButtonIndex = -1;
unsigned long lastArpeggioMillis;
int arpeggioNum = 0;


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 51, en = 47, d4 = 43, d5 = 39, d6 = 35, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lastBtnState = HIGH;
int lastCounterVal = 0;



int wrapIndex(int i, int max) {
  return ((i % max) + max) % max;
}

const int numPages = 4;
int pageIndex = 0;
Page pages[numPages];
Page currentPage;

#define KEY_PAGE 0
#define OCTAVE_PAGE 1
#define INVERSION_PAGE 2
#define PATTERN_PAGE 3

Page keyPage = { "Key", 12, 0, { "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab" } };
Page octavePage = { "Octave", 5, 2, { "1", "2", "3", "4", "5" } };
Page inversionPage = { "Inversion", 3, 0, { "Root", "1st", "2nd" } };
Page patternPage = { "Pattern", 3, 0, { "Apeggio", /*"Alberti",*/ "Blip", "Chirp"/*, "Single"*/} };




int scaleSteps[] = {0, 2, 4, 5, 7, 9, 11, 12};


bool rootNotesPressed[7] = { false, false, false, false, false, false, false };
int chords[8][4] = {
  { 0, 4, 7, 11 },       // I
  { 0, 3, 7, 10 }, // ii
  { 0, 3, 7, 10 }, // iii
  { 0, 4, 7, 11 }, // IV
  { 0, 4, 7, 10 }, // V
  { 0, 3, 7, 10 }, // vi
  { 0, 3, 6, 10 }, // vii dim
  { 0, 4, 7, 11 },
};


float getNoteFreq(int n) {
  return 440.0 * pow(2, (n) / 12.0);
}


void setup() {
  pages[KEY_PAGE] = keyPage;
  pages[OCTAVE_PAGE] = octavePage;
  pages[INVERSION_PAGE] = inversionPage;
  pages[PATTERN_PAGE] = patternPage;
  currentPage = pages[0];

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
  lastArpeggioMillis = millis();
  Serial.begin(9600);
}



void loop() {
  // check when button is first pressed (went from HIGH to LOW)
  if (digitalRead(SW) == LOW && lastBtnState == HIGH) {
    pageIndex++;  // go to next page
    // wraparound index to keep within range
    pageIndex = wrapIndex(pageIndex, numPages);
  }
  lastBtnState = digitalRead(SW);

  Page* currentPage = &pages[pageIndex];
  if (movedCW) {
    currentPage->selectedIndex++;
    movedCW = false;
  }
  if (movedCCW) {
    currentPage->selectedIndex--;
    movedCCW = false;
  }
  // wraparound index to keep within range
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
      currentButtonIndex = i;
      anyPressed = true;
    }
  }
  if (!anyPressed) {
    currentButtonIndex = -1;
  }

  Serial.println(analogRead(DELAY_POT));
  int delay = 25;
  if (pages[PATTERN_PAGE].selectedIndex == 0) {
        delay = 100;
      }
      if (pages[PATTERN_PAGE].selectedIndex == 1) {
        delay = 25;
      }
      if (pages[PATTERN_PAGE].selectedIndex == 2) {
        delay = 1;
      }

  if (currentButtonIndex != -1) {
    int i = currentButtonIndex;
    int keyOffset = pages[KEY_PAGE].selectedIndex;
    int octaveOffSet = ((pages[OCTAVE_PAGE].selectedIndex-3) * 12);

    if (millis() - lastArpeggioMillis > delay) {
      int stepInChord = chords[i][arpeggioNum];

      if (digitalRead(MAJ_MIN_SWITCH) == LOW) {
        if (arpeggioNum == 1) { // swap third
          if (chords[i][arpeggioNum] == 4) { // make major third minor
            stepInChord = 3;
          } else if (chords[i][arpeggioNum] == 3) { // make minor third major
            stepInChord = 4;
          }
        }
      }

      int noteNum = scaleSteps[i] + stepInChord + keyOffset + octaveOffSet;

      float noteFreq = getNoteFreq(noteNum);
      tone(BUZZER, noteFreq);

      arpeggioNum++;
      if (arpeggioNum >= 4) {
        arpeggioNum = 0;
      }
      lastArpeggioMillis = millis();
    }
  } else {
    arpeggioNum = 0;
    noTone(BUZZER);
  }
}
