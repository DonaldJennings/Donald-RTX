
#pragma once
#include "RenderMode.h"

class BinaryRender : public RenderMode {
public:
    BinaryRender() {}

    virtual GeoVec compute_colour(Ray& r, World& world, int depth) const override {
        HitRecord rec;
        if (world.hit(r, Interval(0.001, std::numeric_limits<double>::infinity()), rec)) {
            return GeoVec(1, 0, 0);
        } else {
            return GeoVec(0, 0, 0);
        }
    }
};
