
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "HitRecord.h"
#include "Interval.h"
class Hittable {
public:
    virtual bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const = 0;
};

#endif
