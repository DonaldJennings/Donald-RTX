#include "Camera.h"
#include "SceneParser.h"
#include "World.h"
#include "Sphere.h"
#include "nlohmann/json.hpp"
#include "Ray.h"
#include "RenderMode.h"

#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main()
{
	Camera camera;
	SceneParser scene_parser;
	
	json parsed_scene = scene_parser.parseScene("../Scenes/scene.json");

	World world;

	// Add shapes to world
    world.add(std::make_shared<Sphere>(GeoVec(0.3,0.19,1),0.2));
	// render scene
	camera.render(world, RenderMode::BINARY);
}