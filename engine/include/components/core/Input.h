#ifndef INPUT_H
#define INPUT_H

#include "rapidjson/document.h"
#include "glm/vec2.hpp"

struct  Input {
    enum Action {
        RELEASED,
        PRESED,
        HOLD
    };
    Action keys[349];
    Action mouseButtons[8];
    bool lastKeys[349];
    bool lastMouseButtons[8];

    Input()
    {
        for(int i = 0; i < 349; i++)
        {
            keys[i] = Action::HOLD;
            lastKeys[i] = Action::HOLD;
        }
        for(int i = 0; i < 8; i++)
        {
            mouseButtons[i] = Action::HOLD;
            lastMouseButtons[i] = Action::HOLD;
        }
    }
};

#endif
