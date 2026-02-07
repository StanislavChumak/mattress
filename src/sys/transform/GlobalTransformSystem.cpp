#include "sys/transform/GlobalTransformSystem.h"

#include "comp/ECSWorld.h"
#include "comp/core/ParentChildren.h"
#include "comp/core/Transform.h"

void update_tree(ECSWorld &world, EntityID id, glm::mat4 parentMatrix)
{
    Transform *transform = world.get_comp<Transform>(id);
    Children *children = world.get_comp<Children>(id);

    if(transform)
    {
        transform->update_local_matrix();
        transform->globalMatrix = parentMatrix * transform->localMatrix;
        parentMatrix = transform->globalMatrix;
    }
    if(children)
    {
        for(EntityID child : children->childrenIds)
            update_tree(world, child, parentMatrix);
    }
}

void GlobalTransformSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, transform] : world.view<Transform>())
    {
        if (!transform->dirty)
            continue;

        EntityID rootId = entity;
        Parent *parent = world.get_comp<Parent>(entity);
        while (parent)
        {
            rootId = parent->parentId;
            parent = world.get_comp<Parent>(rootId);
        }

        update_tree(world, rootId, glm::mat4(1.0f));
    }
}