#include "BlinnPhong.h"

GeoVec BlinnPhong::compute_colour(Ray& ray, int depth, World& world) const
{
    if (depth <= 0) 
    {
        return GeoVec(0, 0, 0);
    }
    // Find the closest intersection point
    HitRecord hitRecord;
    if (!world.hit(ray, Interval(0.001, INFINITY), hitRecord))
    {
        return GeoVec(0, 0, 0); // Return black if no intersection
    }

    GeoVec normal = hitRecord.normal;
    
    GeoVec reflection_color(0, 0, 0);
    if (hitRecord.material->isreflective)
    {
        GeoVec reflection_direction = reflect(normalize(ray.direction), normal);
        Ray reflection_ray(hitRecord.point + 0.001 * reflection_direction, reflection_direction);
        HitRecord reflection_hitRecord;
        reflection_color = hitRecord.material->reflectivity * compute_colour(reflection_ray, depth, world);
    }

    GeoVec light_direction = normalize(world.lights.at(0)->position - hitRecord.point);
    GeoVec view_direction = normalize(-hitRecord.point);
    GeoVec half_vector = normalize((light_direction + view_direction));
    double diffuse = std::max(0.0, dot(normal, light_direction));
    double specular = std::pow(std::max(0.0, dot(normal, half_vector)), 2);
    GeoVec diffuse_component = hitRecord.material->diffusecolor * diffuse;
    GeoVec specular_component = hitRecord.material->specularcolor * specular;
    GeoVec ambient_color = GeoVec(1,1,1) * 0.25;
    return ambient_color + (diffuse_component + specular_component + reflection_color);
}