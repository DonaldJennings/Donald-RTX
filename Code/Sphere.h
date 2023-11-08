#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include "Interval.h"
#include <cmath>

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(GeoVec center, double radius)
        : center(center), radius(radius) {}

    bool hit(Ray &r, Interval ray_interval, HitRecord& rec) const override
    {
        GeoVec oc = r.origin - center;
        auto a = r.direction.length_squared();
        auto b = dot(oc, r.direction);
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = b*b - a*c;
        if (discriminant < 0) return false;

        auto root_nature = sqrt(discriminant);
        auto root = (-b - root_nature) / a;
        if (!ray_interval.surrounds(root))
        {
            root = (-b + root_nature) / a;
            if (!ray_interval.surrounds(root))
            {
                return false;
            }
        }

        // Update the HitRecord information
        rec.t = root;
        rec.point = r.at(rec.t);
        GeoVec outward_normal = (rec.point - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }
public:
    GeoVec center;
    double radius;
};
