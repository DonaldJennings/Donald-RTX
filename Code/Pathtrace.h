#include "RenderMode.h"
#include "HitRecord.h"
#include "Ray.h"
#include "GeoVec.h"
class Pathtrace : public RenderMode
{
public:
    Pathtrace() {}
    virtual ~Pathtrace() {}

    GeoVec compute_colour(Ray& ray, World& world, int depth) const override
    {
        if (depth <= 0)
            return GeoVec(0, 0, 0);
        
        HitRecord hitRecord;
        if (!world.hit(ray, Interval(0.001, std::numeric_limits<double>::infinity()), hitRecord))
            return world.backgroundColour;

        Ray scattered_light;
        GeoVec attenuation;

        if (!scatter(ray, hitRecord, scattered_light, attenuation, hitRecord.material))
            return GeoVec(0, 0, 0);

        GeoVec direct_light = GeoVec(0, 0, 0);
        int shadow_samples = 1;
        // Cast a shadow ray towards each light source
        for (const auto& light : world.lights)
        {
            for (int i = 0; i < shadow_samples; ++i)
            {
                GeoVec toLight = light->sample() - hitRecord.point;
                Ray shadowRay(hitRecord.point + hitRecord.normal * 0.01, toLight);
                HitRecord shadowHitRecord;
                if (!world.hit(shadowRay, Interval(0.001, toLight.length()), shadowHitRecord))
                {
                    // Compute the color at the intersection point using the Blinn-Phong shading model
                    direct_light += attenuation * compute_colour(scattered_light, world, depth - 1);   
                }
            }
            direct_light /= shadow_samples;
        }
        return direct_light;
    }

private:
    bool scatter(Ray& ray, HitRecord& rec, Ray& scattered, GeoVec& attenuation, std::shared_ptr<Material> mat) const
    {
      double bias = 0.001;
      GeoVec scattered_origin = rec.point + rec.normal * bias;
      if (mat->isReflective)
      {
          GeoVec reflected = reflect(normalize(ray.direction), rec.normal);
          scattered = Ray(scattered_origin, reflected);
          attenuation = mat->specularColor;
          return (dot(scattered.direction, rec.normal) > 0);
      }
      else if (mat->isRefractive) {
          GeoVec refracted;
          GeoVec outward_normal;
          GeoVec reflected = reflect(ray.direction, rec.normal);
          double ni_over_nt;
          attenuation = GeoVec(1.0, 1.0, 1.0);
          double reflect_prob;
          double cosine;
          if (dot(ray.direction, rec.normal) > 0) {
              outward_normal = -rec.normal;
              ni_over_nt = mat->refractiveIndex;
              cosine = mat->refractiveIndex * dot(ray.direction, rec.normal) / ray.direction.length();
          }
          else {
              outward_normal = rec.normal;
              ni_over_nt = 1.0 / mat->refractiveIndex;
              cosine = -dot(ray.direction, rec.normal) / ray.direction.length();
          }
          if (refract(ray.direction, outward_normal, ni_over_nt, refracted)) {
              reflect_prob = schlick_approximation(cosine, mat->refractiveIndex);
          }
          else {
              scattered = Ray(scattered_origin, reflected);
              reflect_prob = 1.0;
          }
          if (random_double() < reflect_prob) {
              scattered = Ray(scattered_origin, reflected);
          }
          else {
              scattered = Ray(scattered_origin, refracted);
          }
          return true;
      }
      else
      {
        GeoVec target = rec.point + rec.normal + random_in_unit_sphere();
        scattered = Ray(scattered_origin, target - rec.point);
        if (mat->texture != nullptr) {
            std::pair<double, double> uv = rec.shape->compute_uv(rec);
            GeoVec texture_color = mat->texture->sample(uv.first, uv.second);

            // Copilot suggested - blend the texture color with the material color
            double blend_factor = 0.5;  // Adjust this value as needed
            attenuation = blend_factor * texture_color + (1 - blend_factor) * mat->diffuseColor;
        } else {
            // Use the material color
            attenuation = mat->diffuseColor;
        }
        return true;
      }   
    }

    
     // << COPILOT >> 
    double schlick_approximation(double cosine, double ref_idx) const
    {
        double r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }

    // << COPILOT >> 
    GeoVec random_in_unit_sphere() const
    {
        GeoVec p;
        do {
            p = 2.0 * GeoVec(random_double(), random_double(), random_double()) - GeoVec(1, 1, 1);
        } while (p.length_squared() >= 1.0);
        return p;
    }

    double random_double() const
    {
        return rand() / (RAND_MAX + 1.0);
    }
};