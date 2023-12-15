#include "keyRegister.hpp"


void initKeyRegister() {
  pinMode(LOAD, OUTPUT);
  pinMode(CLOCK_ENABLE, OUTPUT);
  pinMode(CLOCK_IN, OUTPUT);
  pinMode(DATA_IN, INPUT);
}


byte keyShiftIn() {
  // pulse load pin
  digitalWrite(LOAD, LOW);
  delayMicroseconds(5);
  digitalWrite(LOAD, HIGH);
  delayMicroseconds(5);
  // shift in data from register
  digitalWrite(CLOCK_IN, HIGH);
  digitalWrite(CLOCK_ENABLE, LOW);
  byte incoming = shiftIn(DATA_IN, CLOCK_IN, LSBFIRST);
  digitalWrite(CLOCK_ENABLE, HIGH);
  return incoming;
}