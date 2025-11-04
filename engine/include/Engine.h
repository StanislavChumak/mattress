#ifndef ENGINE_H
#define ENGINE_H

#include "../src/ECSWorld.h"
#include "../src/systems/SystemManager.h"
#include "../src/systems/input/InputSystem.h"
#include "../src/resources/ResourceManager.h"
#include "components/core/Window.h"
#include "components/core/Game.h"

namespace engine
{
    struct Config
    {
        std::string executablePath;
        std::string nameWindow = "";
        unsigned int pixelScale = 1;
        glm::uvec2 gameSize;

        std::string pathJsonResource = "";
        std::string pathJsonComponent = "";

        bool displayCursor = true;
        float clearColor[4] = {0.f, 0.f, 0.f, 1.f};
        bool depth = true;

        std::string initState;
    };

    class Core 
    {
        Window *window;
        Game *game;
    public:
        ResourceManager resources;
        ECSWorld world;
        SystemManager systems;
        InputSystem inputSystem;
        Core();
        
        bool init(const Config& config);
        void update(float delta);
        void shutdown();

        bool isCloseWindow() { return glfwWindowShouldClose(window->poiter); }
    };
}

#endif
