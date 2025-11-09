#ifndef INPUT_H
#define INPUT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/vec2.hpp"

struct  Input {
    bool keys[349];
    bool mouseButtons[8];
    bool lastKeys[349];
    bool lastMouseButtons[8];

    Input()
    {
        for(int i = 0; i < 349; i++)
        {
            keys[i] = false;
            lastKeys[i] = false;
        }
        for(int i = 0; i < 8; i++)
        {
            mouseButtons[i] = false;
            lastMouseButtons[i] = false;
        }
    }
};

#endif
