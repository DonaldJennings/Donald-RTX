#pragma once
#include <vector>
#include <memory>
#include "Hittable.h"
#include "GeoVec.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Light.h"
#include "nlohmann/json.hpp"
#include "Interval.h"
#

using json = nlohmann::json;

class World {
public:
    GeoVec backgroundColour;
    std::vector<std::shared_ptr<Hittable>> hittables;
    std::vector<std::shared_ptr<Light>> lights;

    void add(std::shared_ptr<Hittable> hittable) {
        hittables.push_back(hittable);
    }

    void add_light(std::shared_ptr<Light> light) {
        lights.push_back(light);
    }

    void clear() {
        hittables.clear();
    }

    bool hit(Ray& ray, Interval ray_interval, HitRecord& hitRecord)
    {
        HitRecord temp_record;
        bool hitAnything = false;
        auto closestSoFar = ray_interval.end();

        for (const auto& hittable : hittables) {
            if (hittable->hit(ray, Interval(ray_interval.start(), closestSoFar), temp_record)) {
                hitAnything = true;
                closestSoFar = temp_record.t;
                hitRecord = temp_record;
            }
        }

        return hitAnything;
    }

    static World fromParsedScene(json scene)
    {
        // Get Hittables
        World world;

        world.backgroundColour = GeoVec(scene["backgroundcolor"][0], scene["backgroundcolor"][1], scene["backgroundcolor"][2]);
        for (auto& shape : scene["shapes"])
        {
            if (shape["type"] == "sphere")
            {
                auto sphere = std::make_shared<Sphere>(GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]), shape["radius"]);
                world.add(sphere);
            }
            else if (shape["type"] == "cylinder")
            {
                auto cylinder = std::make_shared<Cylinder>(
                    GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]), 
                    GeoVec(shape["axis"][0],shape["axis"][1], shape["axis"][2]),
                    shape["radius"],
                    shape["height"]
                );

                world.add(cylinder);
            }
        }
        return world;
    };
};
