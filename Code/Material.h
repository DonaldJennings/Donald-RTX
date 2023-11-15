#pragma once
#include <vector>
#include "GeoVec.h"
#include "Texture.h"
#include "HitRecord.h"
#include<random>

class Material {
public:
    Material() : ks(0.0), kd(0.0), specularExponent(0.0), diffuseColor(0.0, 0.0, 0.0), specularColor(0.0, 0.0, 0.0), 
                 isReflective(false), reflectivity(0.0), isRefractive(false), refractiveIndex(0.0) {}

    Material(double ks, double kd, double specularExponent, GeoVec diffuseColor, GeoVec specularColor, 
             bool isReflective, double reflectivity, bool isRefractive, double refractiveIndex,
             double roughness, double fresnel)
        : ks(ks), kd(kd), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), 
          isReflective(isReflective), reflectivity(reflectivity), isRefractive(isRefractive), refractiveIndex(refractiveIndex),
          roughness(roughness), fresnel(fresnel) {}

    Material(double ks, double kd, double specularExponent, GeoVec diffuseColor, GeoVec specularColor, 
            bool isReflective, double reflectivity, bool isRefractive, double refractiveIndex, Texture input_texture)
        : ks(ks), kd(kd), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), 
          isReflective(isReflective), reflectivity(reflectivity), isRefractive(isRefractive), refractiveIndex(refractiveIndex),
          texture(std::make_shared<Texture>(input_texture)) {}

    Material(double ks, double kd, double specularExponent, GeoVec diffuseColor, GeoVec specularColor, 
            bool isReflective, double reflectivity, bool isRefractive, double refractiveIndex, Texture input_texture,
            double roughness, double fresnel)
        : ks(ks), kd(kd), specularExponent(specularExponent), diffuseColor(diffuseColor), specularColor(specularColor), 
          isReflective(isReflective), reflectivity(reflectivity), isRefractive(isRefractive), refractiveIndex(refractiveIndex),
          texture(std::make_shared<Texture>(input_texture)), roughness(roughness), fresnel(fresnel) {}

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

    // PATH TRACING
    double roughness = 0.5;
    double fresnel = 0.04;
};