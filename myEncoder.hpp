#ifndef ENCODER_H
#define ENCODER_H

// Rotary Encoder Inputs
#define CLK 2
#define DT 3
#define SW 4

void initEncoder();
int getEncoderPosition();
void setEncoderPosition(int num);
int getEncoderPressed();

#endif

