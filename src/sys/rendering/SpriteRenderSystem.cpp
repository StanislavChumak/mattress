#include "sys/rendering/SpriteRenderSystem.h"

#include "comp/ECSWorld.h"
#include "comp/core/Transform.h"
#include "comp/single/Camera.h"
#include "comp/rendering/Sprite.h"

std::shared_ptr<RenderContext> SpriteRenderSystem::context;

InstanceData sprite_to_instance(Transform *transform, Sprite *sprite)
{
    InstanceData date;
    glm::mat4 glob = transform->globalMatrix;

    date.position = glm::vec2(glob[3].x, glob[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(glob[0].x, glob[0].y));
    scale.y = glm::length(glm::vec2(glob[1].x, glob[1].y));
    date.size = sprite->size * scale;

    date.rotation = std::atan2(glob[0].y, glob[0].x);

    date.lbUV = sprite->subTexture.lbVertex;
    date.rtUV = sprite->subTexture.rtVertex;
    date.color = sprite->color;
    date.layer = sprite->layer;

    return std::move(date);
}

void SpriteRenderSystem::update(ECSWorld &world, const double &delta)
{
    context->begin_batches();

    for(auto [entity, transform, sprite] : world.view<Transform, Sprite>())
    {
        if(!sprite->visibility) continue;

        u_int64_t id = sprite->shader->id() | u_int64_t(sprite->texture->id()) << 32;
        context->submit_batch(id, std::move(sprite_to_instance(transform, sprite)));
    }

    context->end_batches();
}