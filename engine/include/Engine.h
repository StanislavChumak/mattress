#ifndef ENGINE_H
#define ENGINE_H

#include "ECSWorld.h"
#include "systems/SystemManager.h"
#include "systems/input/InputSystem.h"
#include "resources/ResourceManager.h"

struct Window;
struct Game;

namespace engine
{
    struct Config
    {
        std::string executablePath;
        std::string nameWindow = "";
        unsigned int pixelScale = 1;
        glm::uvec2 gameSize;

        bool displayCursor = true;
        float clearColor[4] = {0.f, 0.0f, 0.f, 1.f};
        bool depth = true;

        std::string initState;

        int SaundLocationScale = 10;
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
        void loadJsonComponent(std::string pathJsonResource);
        void update(float delta);
        void shutdown();

        bool isCloseWindow();
    };
}

#endif
