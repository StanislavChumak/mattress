#ifndef PARENT_CHILDREN_H
#define PARENT_CHILDREN_H

#include "../external/jsonUtils.h"

#include "ECSWorld.h"

#include <vector>

class ResourceManager;

struct Children
{
    std::vector<EntityID> childrenIds;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource){}
};

struct Parent
{
    EntityID parentId;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::string nameParent = std::string(getVarJSON<std::string_view>(obj["name"]));
        parentId = world.getNamedEntity(nameParent);

        Children *children = world.getComponent<Children>(parentId);
        if(!children)
        {
            world.addComponent(parentId, Children());
            children = world.getComponent<Children>(parentId);
        }
        children->childrenIds.push_back(id);
    }
};

#endif
