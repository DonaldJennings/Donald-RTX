#pragma once
#include "Ray.h"
#include "HitRecord.h"
#include "Material.h"
#include "Interval.h"

class BoundingBox;

class Hittable {
public:
    virtual bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const = 0;

    virtual std::pair<double, double> compute_uv(const HitRecord& rec) const
    {
        return std::make_pair(0, 0);
    };

    virtual BoundingBox bounding_box() const = 0;
};

