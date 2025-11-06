#ifndef CURSOR_H
#define CURSOR_H

#include "glm/vec2.hpp"
#include "../external/jsonUtils.h"

class ResourceManager;

struct Cursor
{
    glm::dvec2 pos;
};

struct CursorFollower
{
    glm::dvec2 offset;
    void fromJson(simdjson::ondemand::object obj, const ResourceManager &resource)
    {
        offset.x = getVarJSON<double>(obj["offset_y"]);
        offset.y = getVarJSON<double>(obj["offset_x"]);
    }
};

#endif
