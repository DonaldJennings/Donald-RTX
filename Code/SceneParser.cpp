#include "SceneParser.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>

using json = nlohmann::json;

SceneParser::SceneParser()
{
}

json SceneParser::parseScene(const char *filename)
{
    // attempt to open file with filename
    std::ifstream sceneFile(filename);

    if (!sceneFile.is_open())
    {
        throw std::exception();
    }

    // read file into json object
    json scene;
    sceneFile >> scene;
    return scene;
}



