#include "SceneParser.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

SceneParser::SceneParser()
{
}

// json parse stream from filename
json SceneParser::parseScene(const char* filename)
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
