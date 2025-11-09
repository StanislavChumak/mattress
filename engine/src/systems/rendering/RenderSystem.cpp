#include "systems/rendering/RenderSystem.h"

#include "ECSWorld.h"
#include "components/rendering/Camera.h"
#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"

#include "glm/mat4x4.hpp"

void RenderSystem::update(ECSWorld &world, const double &delta)
{
    auto *camera = world.getSingleComponent<Camera>();
    if(!camera) return;
    for(auto [entity, transform, sprite] : world.view<Transform, Sprite2D>())
    {
        if(!sprite->visibility) continue;

        sprite->shader->use();

        sprite->VAO.bind();
        sprite->shader->setMatrix4("modelMatrix", transform->worldMatrix);
        sprite->shader->setMatrix4("projectionMatrix", camera->projection);
        sprite->shader->setFloat("layer", sprite->layer);

        sprite->texture->active();
        sprite->texture->bind();

        sprite->EBO.bind();

        sprite->shader->setInt("textureData", 0);

        glDrawElements(GL_TRIANGLES, sprite->EBO.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    
}