#include "Material.h"

using json = nlohmann::json;
Material::Material(const json& material_json)
{
    ks = material_json["ks"];
    kd = material_json["kd"];
    specularexponent = material_json["specularexponent"];
    diffusecolor = GeoVec(material_json["diffusecolor"][0], material_json["diffusecolor"][1], material_json["diffusecolor"][2]);
    specularcolor = GeoVec(material_json["specularcolor"][0], material_json["specularcolor"][1], material_json["specularcolor"][2]);
    isreflective = material_json["isreflective"];
    isrefractive = material_json["isrefractive"];
    reflectivity = material_json["reflectivity"];
    refractiveindex = material_json["refractiveindex"];
}