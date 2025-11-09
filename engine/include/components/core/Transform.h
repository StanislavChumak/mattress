#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../external/jsonUtils.h"

#include "glm/vec2.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef unsigned long EntityID;
class ResourceManager;

struct Transform {
    glm::vec2 position;
    glm::vec2 worldPosition;
    glm::vec2 scaleSize;
    float rotation;

    glm::mat4 localMatrix{1.0f};
    glm::mat4 worldMatrix{1.0f};

    bool dirty{true};

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        position.x = getVarJSON<int64_t>(obj["pos_x"]); 
        position.y = getVarJSON<int64_t>(obj["pos_y"]);
        scaleSize.x = getVarJSON<int64_t>(obj["scaleSize_x"]);
        scaleSize.y = getVarJSON<int64_t>(obj["scaleSize_y"]);
        rotation = getVarJSON<double>(obj["rotation"]);
    }

    void updateLocalMatrix()
    {
        glm::mat4 m(1.0f);
        m = glm::translate(m, glm::vec3(0.5f * scaleSize.x + position.x, 0.5f * scaleSize.y + position.y, 0.0f));
        m = glm::rotate(m, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        m = glm::translate(m, glm::vec3(-0.5f * scaleSize.x, -0.5f * scaleSize.y, 0.0f));
        m = glm::scale(m, glm::vec3(scaleSize, 1.0f));
        localMatrix = m;
        dirty = false;
    }

    void setPosition(glm::vec2 newPosition) { dirty = true; position = newPosition; }
    void setScaleSize(glm::vec2 newScaleSize) { dirty = true; scaleSize = newScaleSize; }
    void setRotation(float newRotation) {dirty = true; rotation = newRotation; }
};

#endif
