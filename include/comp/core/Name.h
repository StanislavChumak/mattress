#ifndef NAME_H
#define NAME_H

#include "jsonUtils.h"

#include "comp/ECSWorld.h"

#include <string>

class ResourceManager;

struct Name
{
    std::string value;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        value = std::string(get_var_json<std::string_view>(obj["value"]));
        world.naming(id, value);
    }
};

#endif
