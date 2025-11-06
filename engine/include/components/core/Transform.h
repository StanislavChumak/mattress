#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../external/jsonUtils.h"
#include "glm/vec2.hpp"

class ResourceManager;

struct Transform {
    glm::ivec2 position;
    glm::ivec2 size;
    float rotation;

    void fromJson(simdjson::ondemand::object obj, ResourceManager &resource)
    {
        position.x = getVarJSON<int64_t>(obj["pos_x"]); 
        position.y = getVarJSON<int64_t>(obj["pos_y"]);
        size.x = getVarJSON<int64_t>(obj["size_x"]);
        size.y = getVarJSON<int64_t>(obj["size_y"]);
        rotation = getVarJSON<double>(obj["rotation"]);
    }
};

#endif
