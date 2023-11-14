#pragma once
#include <vector>
#include <memory>
#include "Hittable.h"
#include "GeoVec.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "nlohmann/json.hpp"
#include "BoundingBox.h"
#include "Interval.h"
#include "Light.h"

using json = nlohmann::json;

class World : Hittable
{
public:
    World() : box(BoundingBox()) {}

    World(std::shared_ptr<Hittable> hittable)
    {
        add(hittable);
    }

    GeoVec backgroundColour;

    std::vector<std::shared_ptr<Hittable>> hittables;
    std::vector<std::shared_ptr<Light>> lights;

    void add(std::shared_ptr<Hittable> hittable)
    {
        hittables.push_back(hittable);
        box = BoundingBox(box, hittable->bounding_box());
    }

    void add_light(std::shared_ptr<Light> light)
    {
        lights.push_back(light);
    }

    void clear()
    {
        hittables.clear();
    }

    bool hit(Ray &ray, Interval ray_interval, HitRecord &hitRecord) const override
    {
        HitRecord temp_hitRecord;
        bool hit_anything = false;
        double closest_so_far = ray_interval.end();
        for (const auto& hittable : hittables)
        {
            if (hittable->hit(ray, Interval(ray_interval.start(), closest_so_far), temp_hitRecord))
            {
                hit_anything = true;
                closest_so_far = temp_hitRecord.t;
                hitRecord = temp_hitRecord;
            }
        }
        return hit_anything;
    }

    BoundingBox bounding_box() const override
    {
        return box;
    }

private:
    BoundingBox box;
};
