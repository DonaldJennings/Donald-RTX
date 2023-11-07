#pragma once
#include <vector>
#include <memory>
#include "Hittable.h"
#include "GeoVec.h"

class World {
public:
    GeoVec backgroundColour;
    std::vector<std::shared_ptr<Hittable>> hittables;

    void add(std::shared_ptr<Hittable> hittable) {
        hittables.push_back(hittable);
    }

    void clear() {
        hittables.clear();
    }

    bool hit(Ray& ray, double t_min, double t_max, HitRecord& hitRecord)
    {
        HitRecord temp_record;
        bool hitAnything = false;
        auto closestSoFar = t_max;

        for (const auto& hittable : hittables) {
            if (hittable->hit(ray, t_min, closestSoFar, temp_record)) {
                hitAnything = true;
                closestSoFar = temp_record.t;
                hitRecord = temp_record;
            }
        }

        return hitAnything;
    }
};
