#include "nlohmann/json.hpp"

using json = nlohmann::json;

class SceneParser
{
public:
    SceneParser();
    static json parseScene(const char *filename);
};