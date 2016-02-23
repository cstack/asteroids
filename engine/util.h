#ifndef UTIL_H
#define UTIL_H

double abs(double x);
void assert(bool expression, const char* message);
double clip(double value, double min, double max);
int sign(double value);
double wrap(double value, double min, double max);
double rand(double min, double max);

#endif
