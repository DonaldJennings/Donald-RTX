#pragma once
#include "GeoVec.h"
#include "Ray.h"
#include "World.h"

class RenderMode {
public:
    virtual ~RenderMode() {}
    virtual GeoVec compute_colour(Ray& ray, int depth, World& world) const = 0;
};
