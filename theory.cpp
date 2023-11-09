#include <Arduino.h>

float getNoteFreq(int n) {
  return 440.0 * pow(2, (n) / 12.0);
}
