#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include "Interval.h"
#include "BoundingBox.h"

#define _USE_MATH_DEFINES
#include <cmath>

class Sphere : public Hittable {
public:
    Sphere(GeoVec center, double radius)
        : center(center), radius(radius)
        {
            box = BoundingBox(center - GeoVec(radius, radius, radius), center + GeoVec(radius, radius, radius));
        };

    void set_material(Material m)
    {
        material = m;
    }
    
    bool hit(Ray &r, Interval ray_interval, HitRecord& rec) const override
    {
        // std::clog << "Material: " << material.diffuseColor.x << material.isRefractive << std::endl;
        double root = find_root(r, ray_interval);
        if (!ray_interval.surrounds(root))
        {
            return false;
        }

        // Update the HitRecord information
        rec.t = root;
        rec.point = r.at(rec.t);
        GeoVec outward_normal = (rec.point - center) / radius;
        rec.material = material;
        rec.shape = std::make_shared<Sphere>(*this);
        rec.set_face_normal(r, outward_normal);

        return true;
    }

    std::pair<double, double> compute_uv(const HitRecord& rec) const override
    {
        double theta = acos(-rec.normal.y);
        double phi = atan2(-rec.normal.z, rec.normal.x) + M_PI;

        return std::make_pair(1 - phi / (2 * M_PI), 1 - theta / M_PI);
    }

    double find_root(Ray& r, Interval ray_interval) const
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
        }
        return root;
    }

    BoundingBox bounding_box() const override
    {
        return box;
    }
public:
    GeoVec center;
    double radius;
    Material material;
    BoundingBox box;
};
