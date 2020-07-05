#ifndef __RANDOM_H
#define __RANDOM_H

#include <linalg/vector.h>

double ranf(void);
double randn(double average, double stdev);
double d_random(double low, double high);

int SelectEpsilonGreedy(const dvector& vec, double epsilon);

#endif
