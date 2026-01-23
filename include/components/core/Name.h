#ifndef NAME_H
#define NAME_H

#include "../external/jsonUtils.h"

#include "ECSWorld.h"

#include <string>

class ResourceManager;

struct Name
{
    std::string value;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        value = std::string(getVarJSON<std::string_view>(obj["value"]));
        world.naming(id, value);
    }
};

#endif
