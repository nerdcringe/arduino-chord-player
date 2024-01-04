#ifndef SIGNALGEN_H
#define SIGNALGEN_H

//========================
//AD9833 Signal Generator
//========================
#include<AD9833.h>
//----------------------------

// Since AD9833 is technically a microcontroller, it communicates with Arduino using SPI
// SPI PINS:
// MOSI  -> SDATA
// SCK   -> SCLK
// FSYNC -> CS

void initSignalGen(uint8_t SPI_chipSelect);
void setWaveform(uint8_t newWaveform);
void setSignalFrequency(int freqHz);
void stopSignal();

#endif