#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "rapidjson/document.h"
#include "glm/vec2.hpp"

#include <iostream>

class ResourceManager;

struct Transform {
    glm::ivec2 position;
    glm::ivec2 size;
    float rotation;

    void fromJson(const rapidjson::Value& j, const ResourceManager &resource)
    {
        position.x = j["pos"].GetArray()[0].GetInt();
        position.y = j["pos"].GetArray()[1].GetInt();
        size.x = j["size"].GetArray()[0].GetInt();
        size.y = j["size"].GetArray()[1].GetInt();
        rotation = j["rotation"].GetFloat();
    }
};

#endif
