#ifndef UTIL_H
#define UTIL_H

void assert(bool expression, const char* message);
double clip(double value, double min, double max);
int round(double value);

#endif
