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

        if (!hitRecord.material->scatter_light(ray, hitRecord, scattered_light, attenuation))
            return GeoVec(0, 0, 0);

        GeoVec direct_light = GeoVec(0, 0, 0);
        // Cast a shadow ray towards each light source
        for (const auto& light : world.lights)
        {
            GeoVec toLight = light->position() - hitRecord.point;
            Ray shadowRay(hitRecord.point + hitRecord.normal * 0.01, toLight);
            HitRecord shadowHitRecord;
            if (!world.hit(shadowRay, Interval(0.001, toLight.length()), shadowHitRecord))
            {
                // Compute the color at the intersection point using the Blinn-Phong shading model
                direct_light += attenuation * compute_colour(scattered_light, world, depth - 1);
                
            }
        }
        return direct_light;
    }
};