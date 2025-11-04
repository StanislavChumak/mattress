#include "Engine.h"

#include "glm/vec2.hpp"

#define BLOCK_SIZE 32
#define SCALE 4
#define GAME_WIDTH 7 * BLOCK_SIZE
#define GAME_HEIGTH 5 * BLOCK_SIZE


int main(int argc, char **argv)
{
    engine::Core engine;

    engine::Config config;
    config.gameSize = glm::vec2(GAME_WIDTH, GAME_HEIGTH);
    config.nameWindow = "Engine";
    config.pixelScale = SCALE;
    config.executablePath = *argv;
    config.pathJsonResource = "res/resources.json";
    config.pathJsonComponent = "res/components.json";
    config.displayCursor = false;

    
    engine.init(config);


    double lastTime = glfwGetTime(), currentTime, delta;

    while (!engine.isCloseWindow())
    {
        currentTime = glfwGetTime();
        delta = currentTime - lastTime;

        engine.update(delta);

        lastTime = currentTime;
    }

    engine.shutdown();

    return 0;
}