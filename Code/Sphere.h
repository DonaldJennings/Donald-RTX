
#include "Hittable.h"
#include "GeoVec.h"
#include <cmath>

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(GeoVec center, double radius) : center(center), radius(radius) {}

    // implement hit function
    bool hit(Ray& ray, double t_min, double t_max, HitRecord& hitRecord) const override
    {
        GeoVec oc = ray.origin - center;
        auto a = ray.direction.dot(ray.direction);
        auto b = 2.0 * oc.dot(ray.direction);
        auto c = oc.dot(oc) - radius * radius;
        auto discriminant = b * b - 4 * a * c;

        if (discriminant > 0)
        {
            auto temp = (-b - sqrt(discriminant)) / (2.0 * a);
            if (temp < t_max && temp > t_min)
            {
                hitRecord.t = temp;
                hitRecord.point = ray.at(hitRecord.t);
                hitRecord.normal = (hitRecord.point - center) / radius;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / (2.0 * a);
            if (temp < t_max && temp > t_min)
            {
                hitRecord.t = temp;
                hitRecord.point = ray.at(hitRecord.t);
                hitRecord.normal = (hitRecord.point - center) / radius;
                return true;
            }
        }
        return false;
    }

public:
    GeoVec center;
    double radius;
};
