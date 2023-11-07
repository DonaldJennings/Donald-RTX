#include "Camera.h"
#include "SceneParser.h"
#include "World.h"
#include "Sphere.h"
#include "nlohmann/json.hpp"
#include "Ray.h"
#include "RenderMode.h"
#include "Cylinder.h"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main()
{
	Camera camera;
	SceneParser scene_parser;
	
	json parsed_scene = scene_parser.parseScene("../Scenes/binary_primitives.json");
	// set background colour to grey
	World world;
	// world.add(std::make_shared<Sphere>(GeoVec(0, 0, 1), 0.5));
	// world.add(std::make_shared<Sphere>(GeoVec(0, -100.5, -1), 100))

	Sphere sphere = Sphere(GeoVec(0, -0.25, 0), 25.1);
	Sphere sphere1 = Sphere(GeoVec(-0.3, 0.59, 1), 0.2);
	Sphere sphere2 = Sphere(GeoVec(0.3, 0.29, 1), 0.2);

	world.add(std::make_shared<Sphere>(sphere));
	world.add(std::make_shared<Sphere>(sphere1));
	world.add(std::make_shared<Sphere>(sphere2));

	// Configure camera
	camera.setFOV(90.0);
	camera.setPosition(GeoVec(0, 0.75, -0.25));
	camera.look_at(GeoVec(0, 0, 1));
	camera.set_up(GeoVec(0, 1, 0));

	// render scene
	camera.render(world, RenderMode::BLINN_PHONG);
}