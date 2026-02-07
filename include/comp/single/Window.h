#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

struct Window {
    GLFWwindow *poiter = nullptr;
    glm::uvec2 size;
    const char *name;
    unsigned int scale;

    Window(const glm::uvec2 &size, const char *name, unsigned int scale)
    :size(size), name(name), scale(scale) {}
};

#endif
