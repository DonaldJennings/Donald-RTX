
#include "RenderMode.h"
#include <iostream>
class BlinnPhong : public RenderMode {
public:
    BlinnPhong() {}
    virtual ~BlinnPhong() {}

    GeoVec compute_colour(Ray& ray, World& world, int depth) const override
    {
        // Initialize color as black
        GeoVec diffuse_color = GeoVec(0, 0, 0);
        GeoVec specular_color = GeoVec(0, 0, 0);
        GeoVec reflected_color = GeoVec(0, 0, 0);
        GeoVec refracted_color = GeoVec(0, 0, 0);
        // Initialize a hit record
        HitRecord hitRecord;

        if (depth <= 0)
        {
            return world.backgroundColour;
        }
        // Check if the ray hits any object in the world
        if (world.hit(ray, Interval(0.001, std::numeric_limits<double>::max()), hitRecord))
        {
            GeoVec N = hitRecord.normal; // Normal at the intersection point
            GeoVec V = normalize(-ray.direction); // Direction to the viewer
            for (auto light : world.lights)
            {
                GeoVec L = normalize(light->position() - hitRecord.point); // Direction to the light source
                
                Ray shadow_ray(hitRecord.point, L);
                HitRecord shadow_hit_record;
                if (!world.hit(shadow_ray, Interval(0.001, std::numeric_limits<double>::max()), shadow_hit_record))
                {
                    // Compute the color at the intersection point using the Blinn-Phong shading model
                    diffuse_color += diffuse(L, N, hitRecord.material.diffuseColor, hitRecord.material.kd) * light->intensity();
                    specular_color += specular(L, N, V, hitRecord.material.specularColor, hitRecord.material.ks, hitRecord.material.specularExponent) * light->intensity();
                }
            }

            // Add the ambient color
            GeoVec ambient_color = hitRecord.material.diffuseColor * 0.1;

            
            // if the object is reflective, compute the reflected ray and recursively compute the color
            if (hitRecord.material.isReflective)
            {
                GeoVec reflected = reflect(ray.direction, hitRecord.normal);
                Ray reflected_ray(hitRecord.point, reflected);
                GeoVec reflected_color_mat = compute_colour(reflected_ray, world, depth - 1);
                reflected_color = reflected_color_mat;

                // Adjust the diffuse and specular color to account for the reflection
                diffuse_color = (1 - hitRecord.material.reflectivity) * diffuse_color;
                specular_color = (1 - hitRecord.material.reflectivity) * specular_color;
            }

            // if the object is refractive, compute the refracted ray and recursively compute the color
            if (hitRecord.material.isRefractive)
            {
                double refractive_index = hitRecord.material.refractiveIndex;
                GeoVec refracted;
                if (refract(ray.direction, hitRecord.normal, refractive_index, refracted))
                {
                    Ray refracted_ray(hitRecord.point, refracted);
                    refracted_color = compute_colour(refracted_ray, world, depth - 1);
                    refracted_color = 1.0 * refracted_color;
                }
            }

            return ambient_color + diffuse_color + specular_color + reflected_color + refracted_color;
        } else {
            // If the ray doesn't hit any object, return the background color
            return world.backgroundColour;
        }
    }

private:
GeoVec diffuse(GeoVec& light_dir, GeoVec& normal, GeoVec& color, float intensity) const
{
    float diffuse_factor = std::max(0.0, dot(light_dir, normal));
    return color * diffuse_factor * intensity;
}

GeoVec specular(const GeoVec& light_dir, const GeoVec& normal, const GeoVec& view_dir, const GeoVec& color, float intensity, float shininess) const
{
    GeoVec halfway_dir = normalize(light_dir + view_dir);
    float specular_factor = std::pow(std::max(0.0, dot(normal, halfway_dir)), shininess);
    return color * specular_factor * intensity;
}


};
