#include "util.h"

#include <iostream>

double abs(double x) {
  return x > 0 ? x : -x;
}

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

int sign(double value) {
  if (value > 0)
    return 1;
  if (value < 0)
    return -1;
  return 0;
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
