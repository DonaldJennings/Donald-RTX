#pragma once
#include "Ray.h"
#include "HitRecord.h"
#include "Material.h"
#include "Interval.h"

class Hittable {
public:
    virtual bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const = 0;
};

