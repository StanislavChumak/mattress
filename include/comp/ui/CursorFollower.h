#ifndef CURSOR_FOLLOWER_H
#define CURSOR_FOLLOWER_H

#include "comp/single/Cursor.h"

struct CursorFollower
{
    glm::dvec2 offset;
    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        offset.x = get_var_json<double>(obj["offset_y"]);
        offset.y = get_var_json<double>(obj["offset_x"]);
    }
};

#endif
