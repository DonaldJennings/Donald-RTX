#include "utils.h"
#include <random>

double random_double()
{
    static thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

GeoVec random_in_unit_disk()
{
    double r = sqrt(random_double());
    double theta = 2 * M_PI * random_double();
    return GeoVec(r * cos(theta), r * sin(theta), 0);
}