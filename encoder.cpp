#include <Arduino.h>
#include "encoder.hpp"


bool movedCW = false;
bool movedCCW = false;

volatile int counter=0;
volatile String currentDir="";
int currentStateCLK;
int lastStateCLK;


void initEncoder() {
  // Set encoder pins as inputs
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), updateEncoder, CHANGE);
}


int getCounter() {
  return counter;
}

void setCounter(int num) {
  counter = num;
}


void updateEncoder() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter--;
      currentDir ="CCW";
      movedCCW = true;
    } else {
      // Encoder is rotating CW so increment
      counter++;
      currentDir ="CW";
      movedCW = true;
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}