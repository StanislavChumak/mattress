#include "systems/physics/GlobalTransformSystem.h"

#include "components/core/ParentChildren.h"
#include "components/core/Transform.h"

#include "ECSWorld.h"

void updateTree(ECSWorld &world, EntityID id, glm::mat4 parentMatrix)
{
    Transform *transform = world.getComponent<Transform>(id);
    Children *children = world.getComponent<Children>(id);

    if(transform)
    {
        if(transform->dirty) transform->updateLocalMatrix();
        transform->worldMatrix = parentMatrix * transform->localMatrix;
        parentMatrix = transform->worldMatrix;
    }
    if(children)
    {
        for(EntityID child : children->childrenIds)
            updateTree(world, child, parentMatrix);
    }
}

void GlobalTransformSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, transform] : world.view<Transform>())
    {
        if (!transform->dirty)
            continue;

        EntityID rootId = entity;
        Parent *parent = world.getComponent<Parent>(entity);
        while (parent)
        {
            rootId = parent->parentId;
            parent = world.getComponent<Parent>(rootId);
        }

        updateTree(world, rootId, glm::mat4(1.0f));
    }
}