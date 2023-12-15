#include <LiquidCrystal_74HC595.h>
#include "page.hpp"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
#define SER 11
#define SRCLK 13
#define RCLK 12
// create LCD interface using 74HC595 shift register
LiquidCrystal_74HC595 lcd(SER, SRCLK, RCLK, 1, 3, 4, 5, 6, 7);