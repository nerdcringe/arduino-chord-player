#include <Arduino.h>
#include "encoder.hpp"
#include "page.hpp"
#include <LiquidCrystal.h>


int wrapIndex(int i, int max) {
  return ((i % max) + max) % max;
}


const int numPages = 4;
int pageIndex = 0;
Page pages[numPages];
Page currentPage;

Page keyPage = {"Key", 12, 0, {"A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"}};
Page octavePage = {"Octave", 5, 2, {"1", "2", "3", "4", "5"}};
Page inversionPage = {"Inversion", 3, 0, {"Root", "1st", "2nd"}};
Page voicePage = {"Voice", 4, 0, {"Sine", "Square", "Sawtooth", "Triangle"}};



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 46, en = 42, d4 = 32, d5 = 30, d6 = 28, d7 = 26;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lastBtnState = HIGH;
int lastCounterVal = 0;


void setup() {
  pages[0] = keyPage;
  pages[1] = octavePage;
  pages[2] = inversionPage;
  pages[3] = voicePage;
  currentPage = pages[0];

  // put your setup code here, to run once:
  initEncoder();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  Serial.begin(9600);
}


void loop() {
  // check when button is first pressed (went from HIGH to LOW)
  if (digitalRead(SW) == LOW && lastBtnState == HIGH) {
    pageIndex++; // go to next page
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
}
