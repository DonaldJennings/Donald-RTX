#include "RenderMode.h"
#include "HitRecord.h"
#include "Ray.h"
#include "GeoVec.h"
#include "utils.h"
class Pathtrace : public RenderMode
{
public:
    Pathtrace()
    {
        name = "pathtrace";
    }

    virtual ~Pathtrace() {}

    GeoVec compute_colour(Ray& ray, World& world, int depth) const override
    {
        if (depth <= 0)
            return GeoVec(0, 0, 0);
        
        HitRecord hitRecord;
    if (world.hit(ray, Interval(0.001, std::numeric_limits<double>::max()), hitRecord))
    {
        Ray scattered;
        GeoVec attenuation;
        if (scatter(ray, hitRecord, scattered, attenuation))
        {
            GeoVec final_colour(0, 0, 0);
            GeoVec scattered_color = compute_colour(scattered, world, depth - 1);
            for (auto& light : world.lights)
            {
                int num_samples = 5; // Number of samples for soft shadows
                GeoVec soft_shadow(0, 0, 0);
                for (int i = 0; i < num_samples; i++)
                {
                    // Sample a point on the light source
                    GeoVec light_sample = light->sample();
                    Ray shadow_ray(hitRecord.point, normalize(light_sample - hitRecord.point));
                    HitRecord shadow_hit_record;
                    if (!world.hit(shadow_ray, Interval(0.001, (light_sample - hitRecord.point).length()), shadow_hit_record))
                    {   
                        GeoVec adjusted_reflection;
                        if (hitRecord.material->isReflective)
                        {
                            adjusted_reflection = attenuation;
                        }
                        else
                        {
                            adjusted_reflection = hitRecord.material->diffuseColor * attenuation;
                        }
                        soft_shadow += light->intensity() * adjusted_reflection * scattered_color;
                    }
                }
                final_colour += soft_shadow / num_samples;
            }
            return final_colour;
        }
    }
    
    // Gradient blue sky
    GeoVec unit_direction = normalize(ray.direction);
    double t = 0.5 * (unit_direction.y - 1.0);
    return (1.0 - t) * GeoVec(1.0, 1.0, 1.0) + t * world.backgroundColour;

    }

private:

    bool scatter(const Ray& ray, const HitRecord& hitRecord, Ray& scattered, GeoVec& attenuation) const
    {
        if (hitRecord.material->isRefractive)
        {
            
            return refractive_brdf(ray, hitRecord, scattered, attenuation);
        }
        else if (hitRecord.material->isReflective)
        {
            return cook_torrance_brdf(ray, hitRecord, scattered, attenuation);
        }
        else
        {
            return lambertian_brdf(ray, hitRecord, scattered, attenuation);
        }
    }

    bool refractive_brdf(const Ray& ray, const HitRecord& hitRecord, Ray& scattered, GeoVec& attenuation) const
    {
        attenuation = hitRecord.material->specularColor;
        double refraction_ratio = hitRecord.front_face ? (1.0/hitRecord.material->refractiveIndex) : hitRecord.material->refractiveIndex;

        
        double cos_theta = fmin(dot(-normalize(ray.direction), hitRecord.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        GeoVec direction;

        auto r0 = (1 - refraction_ratio) / (1 + refraction_ratio);
        r0 = r0 * r0;
        auto reflect_prob = r0 + (1 - r0) * pow((1 - cos_theta), 5);

        if (cannot_refract || reflect_prob > random_double())
            direction = reflect(normalize(ray.direction), hitRecord.normal);
        else
            direction = refract(normalize(ray.direction), hitRecord.normal, refraction_ratio);

        scattered = Ray(hitRecord.point, direction);
        return true;
    }
    
    
    bool cook_torrance_brdf(const Ray& ray, const HitRecord& hitRecord, Ray& scattered, GeoVec& attenuation) const
    {
        // implement cook torrance 

        // 1. Compute the half vector H
        GeoVec V = normalize(ray.direction);
        GeoVec N = normalize(hitRecord.normal);
        GeoVec H = normalize(V + N);

        // 2. Compute the Fresnel term F
        double F = reflectance(V, N, hitRecord.material->refractiveIndex * 1);

        // 3. Compute the geometric term G
        double NdotH = dot(N, H);
        double NdotV = dot(N, V);
        double NdotL = NdotV;
        double VdotH = dot(V, H);
        double G = std::min(1.0, std::min(2.0 * NdotH * NdotV / VdotH, 2.0 * NdotH * NdotL / VdotH));

        // 4. Compute the roughness term D
        double alpha = hitRecord.material->roughness;
        double D = (alpha * alpha) / (M_PI * pow((1 + (alpha * alpha - 1) * NdotH * NdotH), 2));

        double cook_torrance = F * G * D / (4 * NdotV * NdotL);
        
        // Make sure cookTorrance is clamped to [0, 1] if needed
        // std::clog << "CG Value " << cook_torrance << std::endl;
        // Now you can use cookTorrance as a multiplier for your diffuse color
        GeoVec specularColor = hitRecord.material->specularColor;
        GeoVec diffuseColor = hitRecord.material->diffuseColor;
        
        GeoVec reflectedDir = reflect(V, N);  // Assuming you have a function to compute reflection
        scattered = Ray(hitRecord.point, normalize(reflectedDir));
        
        attenuation = GeoVec(1.0, 1.0, 1.0) - F * specularColor * cook_torrance;
        // attenuation = diffuseColor * (1.0 - cook_torrance) + specularColor * cook_torrance;
        return true;
    };

    GeoVec random_in_unit_sphere() const {
        GeoVec p;
        do {
            p = 2.0 * GeoVec(random_double(), random_double(), random_double()) - GeoVec(1, 1, 1);
        } while (p.length_squared() >= 1.0);
        return p;
    }

    double reflectance(const GeoVec& direction, const GeoVec& normal, double refractiveIndex) const {
      // Compute the cosine of the angle between the direction and the normal
      double cosTheta = dot(-direction, normal);

      // Use Schlick's approximation for reflectance
      double r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
      r0 = r0 * r0;
      return r0 + (1 - r0) * pow((1 - cosTheta), 5);
    }

    bool lambertian_brdf(const Ray& ray, const HitRecord& hitRecord, Ray& scattered, GeoVec& attenuation) const
    {
        GeoVec target = hitRecord.normal + random_in_unit_sphere();
        scattered = Ray(hitRecord.point, normalize(target));

        if (hitRecord.material->texture == nullptr)
        {
            attenuation = hitRecord.material->diffuseColor / M_PI;
            return true;
        }

        auto uv = hitRecord.shape->compute_uv(hitRecord);
        GeoVec texture_color = hitRecord.material->texture->sample(uv.first, uv.second);

        attenuation = hitRecord.material->diffuseColor * texture_color;
        return true;
    }
};