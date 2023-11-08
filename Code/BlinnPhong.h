
#pragma once
#include "RenderMode.h"
#include "HitRecord.h"

class BlinnPhong : public RenderMode
{
    public:
        GeoVec compute_colour(Ray& ray, int depth, World& world) const override
        {
            // Find the closest intersection point
            HitRecord hitRecord;
            if (!world.hit(ray, Interval(0.001, INFINITY), hitRecord))
            {
                return GeoVec(0, 0, 0); // Return black if no intersection
            }
            GeoVec normal = hitRecord.normal;
            GeoVec light_direction = normalize(world.lights.at(0)->position - hitRecord.point);
            GeoVec view_direction = normalize(-hitRecord.point);
            GeoVec half_vector = normalize((light_direction + view_direction));

            double diffuse = std::max(0.0, dot(normal, light_direction));
            double specular = std::pow(std::max(0.0, dot(normal, half_vector)), 100);

            return GeoVec(diffuse, diffuse, diffuse) + GeoVec(specular, specular, specular);
        }

};  
