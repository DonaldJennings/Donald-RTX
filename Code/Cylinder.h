#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include "Interval.h"
#include<cmath>

class Cylinder : public Hittable {
public:
    Cylinder(GeoVec center, GeoVec axis, double radius, double height)
        : center(center), axis(axis), radius(radius), height(2.0*height) { this->center.y -= height;}


    void set_material(std::shared_ptr<Material> m)
    {
        material = m;
    }
    
    // define the hit function
    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override
    {
        // calculate t values for the top and bottom caps
        double t_bottom = (center.y - r.origin.y) / r.direction.y;
        double t_top = ((center.y + height) - r.origin.y) / r.direction.y;
        
        // calculate intersection points
        GeoVec p_bottom = r.at(t_bottom);
        GeoVec p_top = r.at(t_top);
        
        // check if the ray's origin is outside the cylinder
        if (r.origin.y < center.y || r.origin.y > center.y + height)
        {
            if (dot(p_bottom - center, p_bottom - center) <= radius * radius)
            {
                // check if the cap is visible
                if (dot(r.direction, -axis) < 0)
                {
                    rec.t = t_bottom;
                    rec.point = p_bottom;
                    rec.normal = -axis;
                    return true;
                }
            }

            // check top cap
            if (dot(p_top - (center + (axis * height)), p_top - (center + (axis * height))) <= radius * radius)
            {
                // check if the cap is visible
                if (dot(r.direction, axis) < 0)
                {
                    rec.t = t_top;
                    rec.point = p_top;
                    rec.normal = axis;
                    return true;
                }
            }
        }
        
        
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
        if (!ray_interval.surrounds(root1) && !ray_interval.surrounds(root2))
        {
            return false;
        }
        // calculate the hit point
        auto hit_point = r.origin + root1 * r.direction;

        // if the hit point is not on the cylinder, return false
        if (hit_point.y < center.y || hit_point.y > center.y + height)
        {
            return false;
        }

        // calculate the normal
        auto outward_normal = normalize(hit_point - center);

        // set the hit record
        rec.t = root1;
        rec.point = hit_point;
        rec.material = material;
        rec.normal = outward_normal;

        return true;
    }

public:
    GeoVec center;
    GeoVec axis;
    double radius;
    double height;
    std::shared_ptr<Material> material;
};
