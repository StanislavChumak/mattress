#ifndef CAMERA_H
#define CAMERA_H

#include "../external/jsonUtils.h"
#include "glad/glad.h"
#include <glm/vec2.hpp>

#include <glm/mat4x4.hpp>

struct Camera
{
    glm::uvec2 offsetViewport = glm::uvec2(0);
    glm::mat4 projection = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    
    void setOffsetViewport(const u_int width, const u_int height, const u_int offsetX, const u_int offsetY)
    {
        glViewport(offsetX, offsetY, width, height);
        offsetViewport.x = offsetX;
        offsetViewport.y = offsetY;
    }
};

#endif
