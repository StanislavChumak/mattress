#ifndef REGISTER_COMPONENT_H
#define REGISTER_COMPONENT_H

#include "../external/jsonUtils.h"

#include <functional>
#include <string>

typedef unsigned long EntityID;
class ResourceManager;
class ECSWorld;

struct TypeInfo {
    std::function<void(EntityID, simdjson::ondemand::object obj,
    ECSWorld&, ResourceManager&)> addComponentFromJson;
};

inline std::unordered_map<std::string, TypeInfo> typeRegistry;


#define REGISTER_COMPONENT(Component) \
typeRegistry[#Component] = { [](EntityID entity, simdjson::ondemand::object obj,\
                              ECSWorld &world, ResourceManager &resource) \
{                                                                               \
    Component c;                                                                \
    c.fromJson(obj, entity, world, resource);                                   \
    world.addComponent<Component>(entity, std::move(c));                        \
}}

#endif
