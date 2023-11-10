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
        GeoVec oc = r.origin - center;
        double a = dot(r.direction, r.direction) - pow(dot(r.direction, axis), 2);
        double half_b = dot(oc, r.direction) - dot(oc, axis) * dot(r.direction, axis);
        double c = dot(oc, oc) - pow(dot(oc, axis), 2) - radius * radius;
        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) return false;
        double sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (-half_b - sqrtd) / a;
        if (root < ray_interval.start() || ray_interval.end() < root) {
            root = (-half_b + sqrtd) / a;
            if (root < ray_interval.start() || ray_interval.end() < root)
                return false;
        }

        GeoVec p = r.at(root);
        if (p.y < center.y - height || p.y > center.y + height) 
        {
            // Check for intersection with top cap
            double t_top = (center.y + height - r.origin.y) / r.direction.y;
            if (t_top < ray_interval.start() || t_top > ray_interval.end()) {
                return false;
            }
            GeoVec p_top = r.at(t_top);
            if (dot(p_top - (center + axis * height), p_top - (center + axis * height)) <= radius * radius) {
                rec.t = t_top;
                rec.point = p_top;
                rec.set_face_normal(r, axis);
                rec.material = mat;
                return true;
            }

            // Check for intersection with bottom cap
            double t_bottom = (center.y - height - r.origin.y) / r.direction.y;
            if (t_bottom < ray_interval.start() || t_bottom > ray_interval.end()) {
                return false;
            }
            GeoVec p_bottom = r.at(t_bottom);
            if (dot(p_bottom - (center - axis * height), p_bottom - (center - axis * height)) <= radius * radius) {
                rec.t = t_bottom;
                rec.point = p_bottom;
                rec.set_face_normal(r, -axis);
                rec.material = mat;
                return true;
            }

            return false;
        }

        rec.t = root;
        rec.point = p;
        GeoVec outward_normal = normalize((p - center) - dot(p - center, axis) * axis);
        rec.set_face_normal(r, outward_normal);
        rec.material = mat;

        return true;
    }
public:
    GeoVec center;
    GeoVec axis;
    double radius;
    double height;
    Material mat;
};
