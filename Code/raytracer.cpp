#include "utils.h"
#include "Camera.h"
#include "SceneParser.h"
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
#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main()
{
	Camera camera;
	SceneParser scene_parser;
	
	json parsed_scene = scene_parser.parseScene("../Scenes/scene.json");

	World world;
	
	// Add shapes to the world
	for (auto& shape : parsed_scene["scene"]["shapes"])
	{
		std::shared_ptr<Material> material = std::make_shared<Material>(Material(shape["material"]));

		if (shape["type"] == "sphere")
		{
			auto sphere = std::make_shared<Sphere>(
				GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]),
				shape["radius"]);
			
			sphere->set_material(material);
			world.add(sphere);
		}
		else if (shape["type"] == "cylinder")
		{
			auto cylinder = std::make_shared<Cylinder>(
				GeoVec(shape["center"][0], shape["center"][1], shape["center"][2]),
				GeoVec(shape["axis"][0], shape["axis"][1], shape["axis"][2]),
				shape["radius"],
				shape["height"]);

			cylinder->set_material(material);
			world.add(cylinder);
		}
		else if (shape["type"] == "triangle")
		{
			auto triangle = std::make_shared<Triangle>(
				GeoVec(shape["v0"][0], shape["v0"][1], shape["v0"][2]),
				GeoVec(shape["v1"][0], shape["v1"][1], shape["v1"][2]),
				GeoVec(shape["v2"][0], shape["v2"][1], shape["v2"][2]));

			triangle->set_material(material);
			world.add(triangle);
		}
		else
		{
			std::cerr << "Shape type not recognised" << std::endl;
		}
	}

	// Add lights to the world
	for (auto& light : parsed_scene["scene"]["lightsources"])
	{
		auto point_light = std::make_shared<Light>(
			GeoVec(light["position"][0], light["position"][1], light["position"][2]),
			GeoVec(light["intensity"][0], light["intensity"][1], light["intensity"][2]));

		world.add_light(point_light);
	}

	camera.setFOV(parsed_scene["camera"]["fov"]);
	camera.set_width(parsed_scene["camera"]["width"]);
	camera.set_height(parsed_scene["camera"]["height"]);
	camera.go_to(GeoVec(parsed_scene["camera"]["position"][0], parsed_scene["camera"]["position"][1], parsed_scene["camera"]["position"][2]));
	camera.look_at(GeoVec(parsed_scene["camera"]["lookAt"][0], parsed_scene["camera"]["lookAt"][1], parsed_scene["camera"]["lookAt"][2]));
	camera.set_up(GeoVec(parsed_scene["camera"]["upVector"][0], parsed_scene["camera"]["upVector"][1], parsed_scene["camera"]["upVector"][2]));
	
	// Set up Render Mode
	BinaryRender binary_render;
	BlinnPhong blinn_phong;

	// render scene
	camera.render(world, blinn_phong);
}