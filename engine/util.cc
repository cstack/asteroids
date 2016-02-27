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

int num_calls_to_rand = 0;
double rand(double min, double max) {
  num_calls_to_rand++;
  double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  return min + (max-min)*r;
}
int get_num_calls_to_rand() { return  num_calls_to_rand; }

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
