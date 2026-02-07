#ifndef PARENT_CHILDREN_H
#define PARENT_CHILDREN_H

#include "jsonUtils.h"

#include "comp/ECSWorld.h"

#include <vector>

class ResourceManager;

struct Children
{
    std::vector<EntityID> childrenIds;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource){}
};

struct Parent
{
    EntityID parentId;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::string nameParent = std::string(get_var_json<std::string_view>(obj["name"]));
        parentId = world.get_name_entity(nameParent);

        Children *children = world.get_comp<Children>(parentId);
        if(!children)
        {
            world.add_comp(parentId, Children());
            children = world.get_comp<Children>(parentId);
        }
        children->childrenIds.push_back(id);
    }
};

#endif
