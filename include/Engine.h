#ifndef ENGINE_H
#define ENGINE_H

#include "comp/ECSWorld.h"
#include "sys/SystemManager.h"
#include "sys/input/InputSystem.h"
#include "res/ResourceManager.h"

struct Window;
struct States;

namespace engine
{
    struct Config
    {
        std::string executablePath;
        std::string nameWindow = "";
        unsigned int pixelScale = 1;
        glm::uvec2 pixelSize;

        bool displayCursor = true;
        float clearColor[4] = {0.2f, 0.0f, 0.3f, 1.f};
        bool depth = true;

        std::string initState;

        int SaundLocationScale = 10;
    };

    class Core 
    {
        Window *window;
        States *states;
    public:
        ResourceManager resources;
        ECSWorld world;
        SystemManager systems;
        InputSystem inputSystem;
        Core();
        
        bool init(const Config& config);
        void load_component(std::string pathJsonResource);
        void pred_upate();
        void update(float delta);
        void shutdown();

        bool is_close_window();
    };
}

#endif
