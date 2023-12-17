#include "keyRegister.hpp"

/*
  74HC165 Shift register input example
 
  https://wokwi.com/arduino/projects/306031380875182657
 
  (C) 2021, Uri Shaked
*/
 
 

#define dataPin 5   /* Q7 */
#define clockPin 6  /* CP */
#define latchPin 7  /* PL */
 
const int numBits = 16;   /* Set to 8 * number of shift registers */
 
void initKeyRegister() {
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
}


int keyShiftIn() {
  // Step 1: Sample
  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);
 
  // Step 2: Shift
  unsigned incoming = 0;
  for (int i = 0; i < numBits; i++) {
    int bit = digitalRead(dataPin);
    if (bit == HIGH) {
      incoming |= 1 << i;
    }
    digitalWrite(clockPin, HIGH); // Shift out the next bit
    digitalWrite(clockPin, LOW);
  }
  delay(5);
  return incoming;
}