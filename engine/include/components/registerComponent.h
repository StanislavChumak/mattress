#ifndef REGISTER_COMPONENT_H
#define REGISTER_COMPONENT_H

#include "../../src/ECSWorld.h"
#include "../../src/resources/ResourceManager.h"

#include "rapidjson/document.h"
#include <functional>
#include <string>
#include <unordered_map>

struct TypeInfo {
    std::function<void(EntityID, const rapidjson::Value&,
    ECSWorld&, const ResourceManager&)> addComponentFromJson;
};

inline std::unordered_map<std::string, TypeInfo> typeRegistry;


#define REGISTER_COMPONENT(Component) \
typeRegistry[#Component] = { [](EntityID entity, const rapidjson::Value& json,  \
                              ECSWorld &world, const ResourceManager &resource) \
{                                                                               \
    Component c;                                                                \
    c.fromJson(json, resource);                                                 \
    world.addComponent<Component>(entity, std::move(c));                        \
}}

#endif
