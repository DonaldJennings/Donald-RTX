#include "GeoVec.h"
#include "Material.h"

struct HitRecord {
    double t; // parameter value of the ray at the hit point
    GeoVec point; // point in 3D space where the ray hit the object
    std::shared_ptr<Material> material_ptr; // material of the object at the hit point
    GeoVec normal; // normal vector of the object at the hit point

    bool front_face;

    // Write a function to set normal face
    inline void set_face_normal(Ray r, GeoVec outward_normal) {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};
