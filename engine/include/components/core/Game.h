#ifndef GAME_H
#define GAME_H

#include "glm/vec2.hpp"

#include <string>

struct Game {
    glm::uvec2 size;
    std::string currentState;
};

#endif
