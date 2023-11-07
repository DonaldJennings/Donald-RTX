
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "HitRecord.h"

class Hittable {
public:
    virtual bool hit(Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif
