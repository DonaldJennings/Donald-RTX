#pragma once
#include "GeoVec.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Material {
public:
    // create constructor from json object
    Material(const json& material_json);
    
    float ks; // specular coefficient
    float kd; // diffuse coefficient
    float specularexponent;
    GeoVec diffusecolor;
    GeoVec specularcolor;
    bool isreflective;
    bool isrefractive;
    float reflectivity;
    float refractiveindex;
};
