#ifndef PAGE_H
#define PAGE_H
#include <Arduino.h>

typedef struct page_struct {
  String title;
  int numOptions;
  int selectedIndex;
  char* options[15];

} Page;


String getOption(Page page, int optionIndex);

#endif