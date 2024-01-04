#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#include "myOLED.hpp"


// You can use any (4 or) 5 pins
//#define sclk 52
//#define mosi 51
//#define cs   10
#define rst  A5
#define dc   12

#define WIDTH 95
#define HEIGHT 63


// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

// Option 1: use any pins but a little slower
//Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

// Option 2: must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1331* display;



int heights[WIDTH];

void initOLED(int SPI_chipSelect) {
  display = &Adafruit_SSD1331(&SPI, SPI_chipSelect, dc, rst);
  display->begin();
  display->fillScreen(BLACK);

  for (int i = 0; i < WIDTH; i++) {
    heights[i] = 0;
  }
}



int x = 0;
void updateOLED(int sensorVal) {
  if (x == WIDTH-1) {
    display->fillScreen(BLACK);
  }
  sensorVal = map(sensorVal, 0, 1023, 32-10, 32+10);
  display->drawPixel(x, sensorVal, GREEN);
  x = (x+1) % WIDTH;
}