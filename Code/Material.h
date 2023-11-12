#pragma once
#include <vector>
#include "GeoVec.h"
#include "Texture.h"
#include "HitRecord.h"

class Material {
public:
    Material() : ks(0.0), kd(0.0), specularExponent(0.0), diffuseColor(0.0, 0.0, 0.0), specularColor(0.0, 0.0, 0.0), 
                 isReflective(false), reflectivity(0.0), isRefractive(false), refractiveIndex(1.0) {}

    Material(double ks, double kd, double specularExponent, GeoVec diffuseColor, GeoVec specularColor, 
             bool isReflective, double reflectivity, bool isRefractive, double refractiveIndex)
        : ks(ks), kd(kd), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), 
          isReflective(isReflective), reflectivity(reflectivity), isRefractive(isRefractive), refractiveIndex(refractiveIndex) {}

    Material(double ks, double kd, double specularExponent, GeoVec diffuseColor, GeoVec specularColor, 
            bool isReflective, double reflectivity, bool isRefractive, double refractiveIndex, Texture input_texture)
        : ks(ks), kd(kd), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), 
          isReflective(isReflective), reflectivity(reflectivity), isRefractive(isRefractive), refractiveIndex(refractiveIndex),
          texture(std::make_shared<Texture>(input_texture)) {}

    double ks;              // The specular reflection constant
    double kd;              // The diffuse reflection constant
    double specularExponent; // The specular exponent
    GeoVec diffuseColor;    // The basic color of the material
    GeoVec specularColor;   // The color of the specular highlight
    bool isReflective;      // Whether the material is reflective
    double reflectivity;    // The reflectivity of the material
    bool isRefractive;      // Whether the material is refractive
    double refractiveIndex; // The refractive index of the material
    std::shared_ptr<Texture> texture;


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

    bool scatter_light(Ray& ray, HitRecord& rec, Ray& scattered, GeoVec& attenuation)
    {
      double bias = 0.001;
      GeoVec scattered_origin = rec.point + rec.normal * bias;
      if (isReflective)
      {
          GeoVec reflected = reflect(normalize(ray.direction), rec.normal);
          scattered = Ray(scattered_origin, reflected);
          attenuation = specularColor;
          return (dot(scattered.direction, rec.normal) > 0);
      }
      else if (isRefractive) {
          GeoVec refracted;
          GeoVec outward_normal;
          GeoVec reflected = reflect(ray.direction, rec.normal);
          double ni_over_nt;
          attenuation = GeoVec(1.0, 1.0, 1.0);
          double reflect_prob;
          double cosine;
          if (dot(ray.direction, rec.normal) > 0) {
              outward_normal = -rec.normal;
              ni_over_nt = refractiveIndex;
              cosine = refractiveIndex * dot(ray.direction, rec.normal) / ray.direction.length();
          }
          else {
              outward_normal = rec.normal;
              ni_over_nt = 1.0 / refractiveIndex;
              cosine = -dot(ray.direction, rec.normal) / ray.direction.length();
          }
          if (refract(ray.direction, outward_normal, ni_over_nt, refracted)) {
              reflect_prob = schlick_approximation(cosine, refractiveIndex);
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
        attenuation = this->diffuseColor;
        return true;
      }   
    }
};