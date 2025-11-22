#include "systems/rendering/SpriteRenderSystem.h"

#include "ECSWorld.h"
#include "components/core/Transform.h"
#include "components/rendering/Camera.h"
#include "components/rendering/Sprite.h"

std::shared_ptr<RenderContext> SpriteRenderSystem::context;

void SpriteRenderSystem::update(ECSWorld &world, const double &delta)
{
    for(auto &batch : context->batches)
        batch.second.beginBatch();

    for(auto [entity, transform, sprite] : world.view<Transform, Sprite2D>())
    {
        if(!sprite->visibility) continue;

        u_int64_t id = sprite->shader->id() | u_int64_t(sprite->texture->id()) << 32;
        context->batches.at(id).submit(sprite, transform);
    }

    for(auto &batch : context->batches)
        batch.second.endBatch();
}