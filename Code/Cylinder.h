#pragma once
#include "Hittable.h"
#include "GeoVec.h"
#include "Ray.h"
#include "Interval.h"
#include<cmath>


class Cylinder : public Hittable {
public:
    Cylinder() {}
    Cylinder(GeoVec center, GeoVec axis, double radius, double height)
        : center(center), axis(axis), radius(radius), height(height) {}

    void set_material(Material mat) { this->mat = mat; }

    bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override {
        double root = find_root(r, ray_interval);
        if (root < ray_interval.start() || ray_interval.end() < root)
        {
            return false;
        }

        GeoVec cylinder_point = r.at(root);

        if (cylinder_point.y < center.y - height || cylinder_point.y > center.y + height)
        {
            if (cylinder_point.y < center.y - height)
            {
                GeoVec cap_center = center - height * axis;
                GeoVec cap_normal = -axis;
                return check_cap_intersection(r, ray_interval, rec, cap_center, cap_normal);
            }
            else
            {
                GeoVec cap_center = center + height * axis;
                GeoVec cap_normal = axis;
                return check_cap_intersection(r, ray_interval, rec, cap_center, cap_normal);
            }
        }

        rec.t = root;
        rec.point = cylinder_point;
        GeoVec outward_normal = normalize((cylinder_point - center) - dot(cylinder_point - center, axis) * axis);
        rec.set_face_normal(r, outward_normal);
        rec.material = mat;

        return true;
    }

    // << COPILOT GENERATED >>
    bool check_cap_intersection(Ray& r, Interval ray_interval, HitRecord& rec, GeoVec cap_center, GeoVec cap_normal) const
    {
        double t = (cap_center.y - r.origin.y) / r.direction.y;
        if (t < ray_interval.start() || t > ray_interval.end())
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
        rec.material = mat;
        return true;
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
};
