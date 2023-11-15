
#include "RenderMode.h"
#include <iostream>
#include "utils.h"
class BlinnPhong : public RenderMode {
public:
    BlinnPhong() 
    {
        name = "BlinnPhong";
    }
    virtual ~BlinnPhong() {}

    GeoVec compute_colour(Ray& ray, World& world, int depth) const override
    {
        GeoVec reflected_color = GeoVec(0, 0, 0);
        GeoVec refracted_color = GeoVec(0, 0, 0);
        
        HitRecord hitRecord;

        // If we've exceed the ray bounce limit or we don't hit anything, return the background colour
        if (depth <= 0)
        {
            return world.backgroundColour;
        }

        if (!world.hit(ray, Interval(0.001, std::numeric_limits<double>::infinity()), hitRecord))
        {
            return world.backgroundColour;
        }

        GeoVec diffuse_color = compute_diffuse_color(hitRecord, world);
        GeoVec specular_color = compute_specular_color(hitRecord, world, ray);
        GeoVec ambient_color = compute_ambient_color(hitRecord);

        if (hitRecord.material->isReflective)
        {
            reflected_color = compute_reflected_color(hitRecord, world, ray, depth);
            diffuse_color = (1 - hitRecord.material->reflectivity) * diffuse_color;
        specular_color = (1 - hitRecord.material->reflectivity) * specular_color;
        }

        if (hitRecord.material->isRefractive)
        {
            refracted_color = compute_refracted_color(hitRecord, world, ray, depth);
            refracted_color = 1.0 * refracted_color;
        }
        
        return ambient_color + diffuse_color + specular_color + reflected_color + refracted_color;
    }

    private:
    // << COPILOT GENERATED >> 
        GeoVec compute_diffuse_color(HitRecord& hitRecord, World& world) const
        {
            GeoVec diffuse_color = GeoVec(0, 0, 0);
            GeoVec N = hitRecord.normal; // Normal at the intersection point
            for (auto light : world.lights)
            {
                GeoVec L = normalize(light->position() - hitRecord.point); // Direction to the light source

                Ray shadow_ray(hitRecord.point, L);
                HitRecord shadow_hit_record;
                if (!world.hit(shadow_ray, Interval(0.001,  (light->position() - hitRecord.point).length()), shadow_hit_record))
                {
                    GeoVec material_color = hitRecord.material->diffuseColor;
                    if (hitRecord.material->texture != nullptr)
                    {
                        if (hitRecord.shape != nullptr)
                        {
                            std::pair<double, double> uv = hitRecord.shape->compute_uv(hitRecord);
                            material_color = hitRecord.material->texture->sample(uv.first, uv.second);
                        }
                    }
                    // Compute the color at the intersection point using the Blinn-Phong shading model
                    diffuse_color += diffuse(L, N, material_color, hitRecord.material->kd) * light->intensity();
                }
            }
            return diffuse_color;
        }

        GeoVec compute_specular_color(HitRecord& hitRecord, World& world, Ray& ray) const
        {
            GeoVec specular_color = GeoVec(0, 0, 0);
            GeoVec N = hitRecord.normal; // Normal at the intersection point
            GeoVec V = normalize(-ray.direction); // Direction to the viewer
            for (auto light : world.lights)
            {
                GeoVec L = normalize(light->position() - hitRecord.point); // Direction to the light source

                Ray shadow_ray(hitRecord.point, L);
                HitRecord shadow_hit_record;
                if (!world.hit(shadow_ray, Interval(0.001, (light->position() - hitRecord.point).length()), shadow_hit_record))
                {
                    // Compute the color at the intersection point using the Blinn-Phong shading model
                    specular_color += specular(L, N, V, hitRecord.material->specularColor, hitRecord.material->ks, hitRecord.material->specularExponent) * light->intensity();
                }
            }
            return specular_color;
        }

        GeoVec compute_ambient_color(const HitRecord& hitRecord) const
        {
            GeoVec ambient =  hitRecord.material->diffuseColor;
            if(hitRecord.material->texture != nullptr)
            {
                if(hitRecord.shape != nullptr)
                {
                    std::pair<double, double> uv = hitRecord.shape->compute_uv(hitRecord);
                    ambient = hitRecord.material->texture->sample(uv.first, uv.second);
                }
            }
            return ambient * 0.01;
        }

        GeoVec compute_reflected_color(HitRecord& hitRecord, World& world, Ray& ray, int depth) const
        {
            GeoVec reflected_color = GeoVec(0, 0, 0);
            GeoVec reflected = reflect(ray.direction, hitRecord.normal);
            Ray reflected_ray(hitRecord.point, reflected);
            GeoVec reflected_color_mat = compute_colour(reflected_ray, world, depth - 1);
            reflected_color = reflected_color_mat;
            return reflected_color;
        }

        GeoVec compute_refracted_color(HitRecord& hitRecord, World& world, Ray& ray, int depth) const
        {
            GeoVec refracted_color = GeoVec(0, 0, 0);
            double refractive_index = hitRecord.material->refractiveIndex;
            GeoVec refracted = refract(ray.direction, hitRecord.normal, refractive_index);

            // Compute Fresnel reflectance
            double cos_theta_i = dot(-ray.direction, hitRecord.normal);
            double fresnel_reflectance = schlick(cos_theta_i, refractive_index);

            if (random_double() < fresnel_reflectance) {
                // Reflect
                GeoVec reflected = reflect(ray.direction, hitRecord.normal);
                Ray reflected_ray(hitRecord.point, reflected);
                refracted_color = compute_colour(reflected_ray, world, depth - 1);
            } else {
                // Refract
                Ray refracted_ray(hitRecord.point, refracted);
                refracted_color = compute_colour(refracted_ray, world, depth - 1);
            }
            return refracted_color;
        }

        GeoVec diffuse(const GeoVec& light_dir, const GeoVec& normal, const GeoVec& color, float intensity) const
        {
            float diffuse_factor = std::max(0.0, dot(light_dir, normal));
            return color * diffuse_factor;
        }

        GeoVec specular(const GeoVec& light_dir, const GeoVec& normal, const GeoVec& view_dir, const GeoVec& color, float intensity, float shininess) const
        {
            GeoVec halfway_dir = normalize(light_dir + view_dir);
            float specular_factor = std::pow(std::max(0.0, dot(normal, halfway_dir)), shininess);
            return color * specular_factor;
        }

        double schlick(double cos_theta_i, double refractive_index) const
        {
            double r0 = (1.0 - refractive_index) / (1.0 + refractive_index);
            r0 = r0 * r0;
            return r0 + (1.0 - r0) * std::pow((1.0 - cos_theta_i), 5.0);
        }

protected:
    std::string name = "BlinnPhong";
};
