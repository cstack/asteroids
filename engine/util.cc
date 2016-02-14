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

double wrap(double value, double min, double max) {
  double range = max - min;
  while (value < min) {
    value += range;
  }
  while (value > max) {
    value -= range;
  }
  return value;
}

int round(double value) {
  return (int) (value + 0.5);
}
