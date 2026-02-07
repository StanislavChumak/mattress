#include "sys/transform/CursorFollowersSystem.h"

#include "comp/ECSWorld.h"
#include "comp/ui/CursorFollower.h"
#include "comp/core/Transform.h"

void CursorFollowersSystem::update(ECSWorld &world, const double &delta)
{
    Cursor *cursor = world.get_single_comp<Cursor>();
    if(!cursor) return;
    for(auto [entity, transform, follower] : world.view<Transform, CursorFollower>())
    {
        transform->set_position(cursor->pos + follower->offset);
    }
}