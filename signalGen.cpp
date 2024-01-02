#include "signalGen.hpp"

AD9833 gen(SPI_CS);
uint8_t currentWaveform = AD9833_SQUARE1;

void initSignalGen() {
  gen.begin();
}

void setWaveform(uint8_t newWaveform) {
  currentWaveform = newWaveform;
}


void setSignalFrequency(int freqHz) {
  gen.setFrequency(freqHz);
  gen.setWave(currentWaveform);
}

void stopSignal() {
  gen.setWave(AD9833_OFF);
}




/*
AD9833 gen(SPI_CS);
WaveformType currentWaveform = SINE_WAVE;

void initSignalGen() {
  gen.Begin();
  gen.Reset();
  gen.EnableOutput(false);
}

void setSignalFrequency(int freqHz) {
  gen.ApplySignal(currentWaveform, REG0, freqHz);
  gen.EnableOutput(true);
  Serial.println(gen.GetActualProgrammedFrequency(REG0));
}

void stopSignal() {
  gen.EnableOutput(false);
}*/
