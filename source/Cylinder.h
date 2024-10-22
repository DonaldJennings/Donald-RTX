#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include "Interval.h"
#include "BoundingBox.h"
#include<cmath>
#include<limits>



class Cylinder : public Hittable {
public:
    Cylinder() {}
    Cylinder(GeoVec center, GeoVec axis, double radius, double height)
        : center(center), axis(axis), radius(radius), height(height) 
        {
            GeoVec min = center - GeoVec(radius, height, radius);
            GeoVec max = center + GeoVec(radius, height, radius);
            box = BoundingBox(min, max);
        }

    void set_material(Material mat) { this->mat = mat; }

    BoundingBox bounding_box() const override
    {
        return box;
    }

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override {
        double root = find_root(r, ray_interval);
        if (root < ray_interval.start() || ray_interval.end() < root)
        {
            return false;
        }

        GeoVec cylinder_point = r.at(root);
        
        GeoVec vector_to_point = cylinder_point - center;
        double projection = dot(vector_to_point, axis);
        if (projection < - height || projection > height)
        {
            if (projection >= 0)
            {
                GeoVec cap_center = center + height * axis;
                GeoVec cap_normal = normalize(axis);
                return check_cap_intersection(r, ray_interval, rec, cap_center, cap_normal);
            }
            else
            {
                GeoVec cap_center = center - height * axis;
                GeoVec cap_normal = -normalize(axis);
                return check_cap_intersection(r, ray_interval, rec, cap_center, cap_normal);
            }
        }

        rec.t = root;
        rec.point = cylinder_point;
        GeoVec outward_normal = normalize((cylinder_point - center) - dot(cylinder_point - center, axis) * axis);
        rec.set_face_normal(r, outward_normal);
        rec.shape = std::make_shared<Cylinder>(*this);
        rec.material = mat;

        return true;
    }

    // << COPILOT GENERATED >>
    bool check_cap_intersection(Ray& r, Interval ray_interval, HitRecord& rec, GeoVec cap_center, GeoVec cap_normal) const
    {
        double t = dot(cap_center - r.origin, cap_normal) / dot(r.direction, cap_normal);
        if (t <= ray_interval.start() || t >= ray_interval.end())
        {
            return false;
        }

        GeoVec p = r.at(t);
        if (dot(p - cap_center, p - cap_center) > radius * radius) 
        {
            return false;
        }

        // Set hit record
        rec.t = t;
        rec.point = p;
        rec.set_face_normal(r, cap_normal);
        rec.shape = std::make_shared<Cylinder>(*this);
        rec.material = mat;
        return true;
    }

    std::pair<double, double> compute_uv(const HitRecord& rec) const override
    {
        GeoVec p = rec.point - center;

        // Check if the intersection point is on the top or bottom cap
        if (std::abs(p.y - height) < std::numeric_limits<double>::epsilon() || std::abs(p.y + height) < std::numeric_limits<double>::epsilon()) {
            double u = 0.5 + atan2(p.z, p.x) / (2 * M_PI);
            double v = 0.5 - sqrt(p.x * p.x + p.z * p.z) / (2 * radius);
            return std::make_pair(u, v);
        }

        double theta = atan2(p.z, p.x);
        double u = (theta + M_PI) / (2 * M_PI);
        double v = 1.0 - p.y / height;

        return std::make_pair(u, v);
    }
    
    double find_root(Ray& r, Interval ray_interval) const
    {
        GeoVec oc = r.origin - center;
        double a = dot(r.direction, r.direction) - pow(dot(r.direction, axis), 2);
        double half_b = dot(oc, r.direction) - dot(oc, axis) * dot(r.direction, axis);
        double c = dot(oc, oc) - pow(dot(oc, axis), 2) - radius * radius;
        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return -1;
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (-half_b - sqrtd) / a;
        if (root < ray_interval.start() || ray_interval.end() < root)
        {
            root = (-half_b + sqrtd) / a;
        }

        return root;
    }

public:
    GeoVec center;
    GeoVec axis;
    double radius;
    double height;
    Material mat;
    BoundingBox box;
};
