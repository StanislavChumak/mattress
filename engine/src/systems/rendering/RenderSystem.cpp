#include "systems/rendering/RenderSystem.h"

#include "ECSWorld.h"
#include "components/rendering/Camera.h"
#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

void RenderSystem::update(ECSWorld &world, const double &delta)
{
    auto &camera = world.getSingleComponent<Camera>();
    for(auto [entity, transform, sprite] : world.view<Transform, Sprite2D>())
    {
        if(!sprite->visibility) continue;

        sprite->shader->use();

        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(0.5f * transform->size.x + transform->position.x, 0.5f * transform->size.y + transform->position.y, 0.0f));
        model = glm::rotate(model, glm::radians(transform->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * transform->size.x, -0.5f * transform->size.y, 0.0f));
        model = glm::scale(model, glm::vec3(transform->size, 1.0f));

        sprite->VAO.bind();
        sprite->shader->setMatrix4("modelMatrix", model);
        sprite->shader->setMatrix4("projectionMatrix", camera.projection);
        sprite->shader->setFloat("layer", sprite->layer);

        sprite->texture->active();
        sprite->texture->bind();

        sprite->EBO.bind();

        sprite->shader->setInt("textureData", 0);

        glDrawElements(GL_TRIANGLES, sprite->EBO.get_count(), GL_UNSIGNED_INT, nullptr);
    }

    
}