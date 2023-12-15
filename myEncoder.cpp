
#include <Arduino.h>
#include "myEncoder.hpp"

volatile int position = 0;
int currentStateCLK;
int lastStateCLK;

void updateEncoder();

void initEncoder() {
  // Set encoder pins as inputs
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	pinMode(SW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DT), updateEncoder, CHANGE);
}


int getEncoderPosition() {
  return position;
}

void setEncoderPosition(int num) {
  position = num;
}


void updateEncoder() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);
  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    
    //Serial.println("CLK Change");
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      //Serial.println("Decrement");
      position--;
    } else {
      // Encoder is rotating CW so increment
      position++;
      //Serial.println("Increment");
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}


int getEncoderPressed() {
  return digitalRead(SW);
}