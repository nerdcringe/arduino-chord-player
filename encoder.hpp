#ifndef ENCODER_H
#define ENCODER_H

// Rotary Encoder Inputs
#define CLK 20
#define DT 19
#define SW 18

extern bool movedCW;
extern bool movedCCW;

void initEncoder();
int getCounter();
void setCounter(int num);
void updateEncoder();

#endif
