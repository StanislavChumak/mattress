#include "systems/transform/CursorFollowersSystem.h"

#include "components/ui/Cursor.h"
#include "components/core/Transform.h"
#include "ECSWorld.h"

void CursorFollowersSystem::update(ECSWorld &world, const double &delta)
{
    Cursor *cursor = world.getSingleComponent<Cursor>();
    if(!cursor) return;
    for(auto [entity, transform, follower] : world.view<Transform, CursorFollower>())
    {
        transform->setPosition(cursor->pos + follower->offset);
    }
}