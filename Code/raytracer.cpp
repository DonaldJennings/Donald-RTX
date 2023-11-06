#include "Camera.h"
#include "SceneParser.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
	const char *filename = "../Scenes/scene.json";
	json scene = SceneParser::parseScene(filename);

	Camera camera;
	camera.render();
}