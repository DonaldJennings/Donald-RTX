#include "Camera.h"
#include "World.h"
#include "Sphere.h"
#include "nlohmann/json.hpp"
#include "Ray.h"
#include "RenderMode.h"
#include "Cylinder.h"
#include "GeoVec.h"
#include "BinaryRender.h"
#include "BlinnPhong.h"
#include "Triangle.h"
#include "Material.h"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

json parseScene(const char* filename)
{
	// open file
	std::ifstream scene_file(filename);
	if (!scene_file.is_open())
	{
		std::cerr << "Error: Could not open scene file" << std::endl;
		return json();
	}
	// parse scene
	json scene;
	scene_file >> scene;
	return scene;
}

int main()
{
	Camera camera;
	
	json parsed_scene = parseScene("../Scenes/simple_phong.json");

	World world;
	
	// Add shapes to the world
	for (auto& shape : parsed_scene["scene"]["shapes"])
	{
		auto material = shape["material"];
		double ks = material["ks"].get<double>();
		double kd = material["kd"].get<double>();
		int specularexponent = material["specularexponent"].get<int>();
		GeoVec diffusecolor = GeoVec(material["diffusecolor"][0], material["diffusecolor"][1], material["diffusecolor"][2]);
		GeoVec specularcolor = GeoVec(material["specularcolor"][0], material["specularcolor"][1], material["specularcolor"][2]);
		bool isreflective = material["isreflective"].get<bool>();
		bool isrefractive = material["isrefractive"].get<bool>();
		double reflectivity = material["reflectivity"].get<double>();
		double refractiveindex = material["refractiveindex"].get<double>();

		Material material_obj(ks, kd, specularexponent, diffusecolor, specularcolor, isreflective, reflectivity, isrefractive, refractiveindex);
		
		if (shape["type"] == "sphere")
		{
			auto center = GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]);
			double radius = shape["radius"].get<double>();

			Sphere sphere(center, radius);
			sphere.set_material(material_obj);

			world.add(std::make_shared<Sphere>(sphere));
		}
		else if (shape["type"] == "cylinder")
		{
			auto center { GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]) };
			auto axis { GeoVec(shape["axis"][0], shape["axis"][1], shape["axis"][2]) };
			double radius { shape["radius"].get<double>() };
			double height { shape["height"].get<double>() };

			Cylinder cylinder(center, axis, radius, height);
			cylinder.set_material(material_obj);

			world.add(std::make_shared<Cylinder>(cylinder));
		}
		else if (shape["type"] == "triangle")
		{
			auto v0 { GeoVec(shape["v0"][0], shape["v0"][1], shape["v0"][2]) };
			auto v1 { GeoVec(shape["v1"][0], shape["v1"][1], shape["v1"][2]) };
			auto v2 { GeoVec(shape["v2"][0], shape["v2"][1], shape["v2"][2]) };

			Triangle triangle(v0, v1, v2);
			triangle.set_material(material_obj);
			
			world.add(std::make_shared<Triangle>(triangle));
		}
		else
		{
			std::cerr << "Shape type not recognised" << std::endl;
		}
	}

	// Add lights to the world
	for (auto& light : parsed_scene["scene"]["lightsources"])
	{
		auto position = GeoVec(light["position"][0], light["position"][1], light["position"][2]);
		auto intensity = GeoVec(light["intensity"][0], light["intensity"][1], light["intensity"][2]);

		world.add_light(std::make_shared<Light>(position, intensity));
	}

	std::clog << "Number of lights: " << world.lights.size() << "\n";

	world.backgroundColour = GeoVec(parsed_scene["scene"]["backgroundcolor"][0], parsed_scene["scene"]["backgroundcolor"][1], parsed_scene["scene"]["backgroundcolor"][2]);

	camera.setFOV(parsed_scene["camera"]["fov"]);
	camera.set_width(parsed_scene["camera"]["width"]);
	camera.set_height(parsed_scene["camera"]["height"]);
	camera.set_exposure(parsed_scene["camera"]["exposure"]);
	camera.set_num_bounces(parsed_scene["nbounces"]);
	camera.go_to(GeoVec(parsed_scene["camera"]["position"][0], parsed_scene["camera"]["position"][1], parsed_scene["camera"]["position"][2]));
	camera.look_at(GeoVec(parsed_scene["camera"]["lookAt"][0], parsed_scene["camera"]["lookAt"][1], parsed_scene["camera"]["lookAt"][2]));
	camera.set_up(GeoVec(parsed_scene["camera"]["upVector"][0], parsed_scene["camera"]["upVector"][1], parsed_scene["camera"]["upVector"][2]));
	
	// Set up Render Mode
	BinaryRender binary_render;
	BlinnPhong blinn_phong;
	// render scene
	camera.render(world, blinn_phong);
}