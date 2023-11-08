#pragma once
#include "Ray.h"
#include "World.h"
#include "GeoVec.h"

class RenderMode {
public:
    virtual ~RenderMode() {}
    virtual GeoVec compute_colour(Ray& ray, World& world, int depth) const = 0;
};
