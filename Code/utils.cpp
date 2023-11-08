
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include <cmath>
// Declare any necessary functions or classes that will be defined in utils.cpp

double rand_double()
{
    return rand() / (RAND_MAX + 1.0);
}

double rand_double(double min, double max)
{
    return min + (max-min)*rand_double();
}

#endif // UTILS_H
