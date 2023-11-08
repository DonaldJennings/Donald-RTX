
#include "RenderMode.h"

class BlinnPhong : public RenderMode {
public:
    BlinnPhong() {}
    virtual ~BlinnPhong() {}

    GeoVec compute_colour(Ray& ray, World& world, int depth) const override
    {
        // Light position
        GeoVec light_pos(0.0, 1.0, 0.5);
        GeoVec light_intensity(0.5, 0.5, 0.5);

        // Initialize color as black
        GeoVec color(0.0, 0.0, 0.0);

        // If depth is 0, return black color
        if (depth == 0)
            return color;

        // Initialize a hit record
        HitRecord hitRecord;

        // Check if the ray hits any object in the world
        if (world.hit(ray, Interval(0.001, std::numeric_limits<double>::max()), hitRecord))
        {
            // Compute the color at the intersection point using the Blinn-Phong shading model
            GeoVec N = hitRecord.normal; // Normal at the intersection point
            GeoVec V = -ray.direction; // Direction to the viewer
            GeoVec L = light_pos - hitRecord.point; // Direction to the light
            L = normalize(L); // Normalize the direction to the light

            GeoVec diffuse_color = diffuse(L, N, hitRecord.material.diffuseColor, 0.5);
            GeoVec specular_color = specular(L, N, V, hitRecord.material.specularColor, 0.5, 0.5);

            color = GeoVec(0.2, 0.2, 0.2) + diffuse_color + specular_color;
        }

        return color;
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
