#ifndef CURSOR_H
#define CURSOR_H

#include "rapidjson/document.h"
#include "glm/vec2.hpp"

class ResourceManager;

struct Cursor
{
    glm::dvec2 pos;
};

struct CursorFollower
{
    glm::dvec2 offset;
    void fromJson(const rapidjson::Value& j, const ResourceManager &resource)
    {
        offset.x = j["offset"].GetArray()[0].GetDouble();
        offset.y = j["offset"].GetArray()[1].GetDouble();
    }
};

#endif
