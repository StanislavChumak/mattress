#ifndef CAMERA_H
#define CAMERA_H

#include "../external/jsonUtils.h"
#include "glad/glad.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resources/BufferObject.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

struct Camera
{
    glm::vec2 pixelSize = glm::vec2(0);
    glm::uvec2 offsetViewport = glm::uvec2(0);
    
    BufferObject UBO;
    struct {
        glm::mat4 projection;
        glm::mat4 view;
    } matrices;

    EntityID target = NULL_ENTITY;

    Camera() = delete;
    ~Camera() = default;
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera&) = delete;
    Camera(Camera &&other) noexcept
    {
        UBO = std::move(other.UBO);
        pixelSize = other.pixelSize;
        offsetViewport = other.offsetViewport;
        matrices = other.matrices;
        target = other.target;
        other.target = NULL_ENTITY;
    }
    Camera &operator=(Camera &&other) noexcept
    {
        UBO = std::move(other.UBO);
        pixelSize = other.pixelSize;
        offsetViewport = other.offsetViewport;
        matrices = other.matrices;
        target = other.target;
        other.target = NULL_ENTITY;
        return *this;
    }

    Camera(glm::uvec2 pixelSize)
    : pixelSize(pixelSize)
    {
        UBO.init(GL_UNIFORM_BUFFER, &matrices, sizeof(matrices), GL_DYNAMIC_DRAW);
        UBO.bindBase(0);
    }
    
    void setOffsetViewport(const u_int width, const u_int height, const u_int offsetX, const u_int offsetY)
    {
        glViewport(offsetX, offsetY, width, height);
        offsetViewport.x = offsetX;
        offsetViewport.y = offsetY;
    }

    void updateUBO()
    {
        UBO.update(&matrices, sizeof(matrices));
    }

    void updateProjectionMatrix()
    {
        matrices.projection = glm::ortho(0.0f, pixelSize.x, 0.0f, pixelSize.y, -100.f, 100.f);
        updateUBO();
    }

    void updateViewMatrix(glm::mat4 view)
    {
        matrices.view = std::move(view);
        updateUBO();
    }
};

#endif
