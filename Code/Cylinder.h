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
            GeoVec axis_normalized = normalize(axis);
            GeoVec top = center + height * 0.5 * axis_normalized;
            GeoVec bottom = center - height * 0.5 * axis_normalized;
        
            // Find two vectors perpendicular to the axis
            GeoVec perp1 = normalize(cross(axis_normalized, abs(axis_normalized.x) > 0.1 ? GeoVec(0, 1, 0) : GeoVec(1, 0, 0)));
            GeoVec perp2 = cross(axis_normalized, perp1);
        
            // Calculate the minimum and maximum points
            GeoVec min = GeoVec(std::min(top.x, bottom.x) - radius, std::min(top.y, bottom.y) - radius, std::min(top.z, bottom.z) - radius);
            GeoVec max = GeoVec(std::max(top.x, bottom.x) + radius, std::max(top.y, bottom.y) + radius, std::max(top.z, bottom.z) + radius);
        
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
        rec.material = std::make_shared<Material>(mat);

    return true;
    }

    // << COPILOT GENERATED >>
    bool check_cap_intersection(Ray& r, Interval ray_interval, HitRecord& rec, GeoVec cap_center, GeoVec cap_normal) const
    {
        double t = dot(cap_center - r.origin, cap_normal) / dot(r.direction, cap_normal);
        if (t < ray_interval.start() - 1e-6 || ray_interval.end() + 1e-6 < t)
        {
            return false;
        }

        GeoVec p = r.at(t);

        if ((p - cap_center).length_squared() > (radius * radius + 1e-6))
        {
            return false;
        }

        rec.t = t;
        rec.point = p;
        rec.set_face_normal(r, cap_normal);
        rec.shape = std::make_shared<Cylinder>(*this);
        rec.material = std::make_shared<Material>(mat);

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
        GeoVec dir = normalize(r.direction);
        double a = dot(dir, dir) - pow(dot(dir, axis), 2);
        double half_b = dot(oc, dir) - dot(oc, axis) * dot(dir, axis);
        double c = dot(oc, oc) - pow(dot(oc, axis), 2) - radius * radius;
        double discriminant = half_b * half_b - a * c;
        
        if (discriminant < 0) return -1;
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (-half_b - sqrtd) / a;
        if (root < ray_interval.start() || ray_interval.end() < root)
        {
            root = (-half_b + sqrtd) / a;
            if (root < ray_interval.start() || ray_interval.end() < root)
            {
                return -1;
            }
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
