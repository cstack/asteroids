#include "util.h"

#include <iostream>

void assert(bool expression, const char* message) {
  if (!expression) {
    std::cout << message << std::endl;
    exit(1);
  }
}

double clip(double value, double min, double max) {
  if (value < min)
    value = min;
  if (value > max)
    value = max;
  return value;
}

int round(double value) {
  return (int) (value + 0.5);
}
