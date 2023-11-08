#pragma once
#include "GeoVec.h"

class Light {
public:
    GeoVec position;
    GeoVec intensity;

    Light(GeoVec pos, GeoVec inten) : position(pos), intensity(inten) {}
};
