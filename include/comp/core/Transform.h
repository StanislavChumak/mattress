#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "jsonUtils.h"

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef unsigned long EntityID;
class ECSWorld;
class ResourceManager;

struct Transform
{
    glm::vec2 position;
    glm::vec2 scaleSize;
    float rotation;

    glm::mat4 localMatrix{1.0f};
    glm::mat4 globalMatrix{1.0f};

    bool dirty{true};

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        position.x = get_var_json<double>(obj["pos_x"]); 
        position.y = get_var_json<double>(obj["pos_y"]);
        scaleSize.x = get_var_json<double>(obj["scaleSize_x"]);
        scaleSize.y = get_var_json<double>(obj["scaleSize_y"]);
        rotation = get_var_json<double>(obj["rotation"]);
    }

    void update_local_matrix()
    {
        localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        localMatrix = glm::rotate(localMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        localMatrix = glm::scale(localMatrix, glm::vec3(scaleSize, 1.0f));
        dirty = false;
    }

    void set_position(glm::vec2 newPosition) { dirty = true; position = newPosition; }
    void set_scale_size(glm::vec2 newScaleSize) { dirty = true; scaleSize = newScaleSize; }
    void set_rotation(float newRotation) {dirty = true; rotation = newRotation; }
};

#endif
