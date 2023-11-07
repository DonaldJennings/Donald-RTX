#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include<cmath>

class Cylinder : public Hittable {
public:
    Cylinder() {}
    Cylinder(GeoVec center, GeoVec axis, double radius, double height)
        : center(center), axis(axis), radius(radius), height(height) {}

    // define the hit function
    bool hit(Ray& r, double t_min, double t_max, HitRecord& rec) const override
    {
        // define the quadratic equation
        auto a = pow(r.direction.x, 2) + pow(r.direction.z, 2);
        auto b = 2 * (r.direction.x * (r.origin.x - center.x) + r.direction.z * (r.origin.z - center.z));
        auto c = pow(r.origin.x - center.x, 2) + pow(r.origin.z - center.z, 2) - pow(radius, 2);

        // calculate the discriminant
        auto discriminant = pow(b, 2) - 4 * a * c;

        // if the discriminant is less than 0, there are no real roots
        if (discriminant < 0)
        {
            return false;
        }

        // calculate the roots
        auto root1 = (-b - sqrt(discriminant)) / (2 * a);
        auto root2 = (-b + sqrt(discriminant)) / (2 * a);

        // if the roots are less than t_min or greater than t_max, return false
        if (root1 < t_min || root1 > t_max)
        {
            return false;
        }

        // if the roots are less than t_min or greater than t_max, return false
        if (root2 < t_min || root2 > t_max)
        {
            return false;
        }

        // if the roots are equal, return false
        if (root1 == root2)
        {
            return false;
        }

        // calculate the hit point
        auto hit_point = r.origin + root1 * r.direction;

        // if the hit point is not within the height of the cylinder, return false
        if (hit_point.y < center.y || hit_point.y > center.y + height)
        {
            return false;
        }

        // calculate the normal
        auto outward_normal = (hit_point - center).normalize();

        // set the hit record
        rec.t = root1;
        rec.point = hit_point;
        rec.normal = outward_normal;

        return true;
    }

public:
    GeoVec center;
    GeoVec axis;
    double radius;
    double height;
};
