
#include "Hittable.h"
#include "Material.h"
#include "GeoVec.h"
#include "Interval.h"
#include "Ray.h"
#include "HitRecord.h" 
#include "BoundingBox.h"
#include <memory>


class Plane : public Hittable {
public:
    Plane() {}
    Plane(const GeoVec& point, const GeoVec& normal, Material material)
        : point(point), normal(normal), material(std::make_shared<Material>(material)) {}

    virtual bool hit(Ray& r, Interval ray_interval, HitRecord& rec) const override {
        double t = dot(point - r.origin, normal) / dot(r.direction, normal);
        if (t > ray_interval.start() && t < ray_interval.end()) {
                rec.t = t;
                rec.point = r.origin + t * r.direction;
                rec.material = material;
                rec.normal = normal;
                return true;
            }
        return false;
    }

    virtual BoundingBox bounding_box() const override {
        return BoundingBox(point, point);
    }
public:
    GeoVec point;
    GeoVec normal;
    std::shared_ptr<Material> material;
};
