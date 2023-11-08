#pragma once
#include "GeoVec.h"
#include "Material.h"
#include <memory>

struct HitRecord {
    double t; // parameter value of the ray at the hit point
    GeoVec point; // point in 3D space where the ray hit the object
    GeoVec normal; // normal vector of the object at the hit point
    std::shared_ptr<Material> material; // material of the object
    bool front_face;

    // Write a function to set normal and front_face
    inline void set_face_normal(const Ray& ray, const GeoVec& outward_normal)
    {
        front_face = dot(ray.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
