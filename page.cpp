#include "page.hpp"

String getOption(Page page, int optionIndex) {
  if (optionIndex < page.numOptions) {
    return page.options[optionIndex];
  }
  return "ERROR: No option";
}