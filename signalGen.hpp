#ifndef SIGNALGEN_H
#define SIGNALGEN_H

//========================
//AD9833 Signal Generator
//========================
#include<AD9833.h>
//----------------------------

#define SPI_CS 8 // chip select pin for ad9833

// Since AD9833 is technically a microcontroller, it communicates with Arduino using SPI
// SPI PINS:
// MOSI  -> SDATA
// SCK   -> SCLK
// FSYNC -> CS

void initSignalGen();
void setWaveform(uint8_t newWaveform);
void setSignalFrequency(int freqHz);
void stopSignal();

#endif