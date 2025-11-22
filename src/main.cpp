#include "Engine.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#define INCLUDE_CORE_COMPONENTS
#define INCLUDE_SPRITE
#define INCLUDE_ANIMATION
#define INCLUDE_SOUNDS
#define INCLUDE_LOCATION_SOUNDS

#include "presets.h"


#include "components/ui/Cursor.h"
#include "systems/transform/CursorFollowersSystem.h"

#define BLOCK_SIZE 32
#define SCALE 3
#define GAME_WIDTH 8 * BLOCK_SIZE
#define GAME_HEIGTH 6 * BLOCK_SIZE

class SoundCursorSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

int main(int argc, char **argv)
{
    engine::Core engine;

    engine::Config config;
    config.pixelSize = glm::vec2(GAME_WIDTH, GAME_HEIGTH);
    config.nameWindow = "Engine";
    config.pixelScale = SCALE;
    config.executablePath = *argv;
    config.displayCursor = false;

    REGISTER_CORE_COMPONENTS(engine);
    REGISTER_SPRITE(engine);
    REGISTER_ANIMATION(engine);
    REGISTER_SOUNDS(engine);
    REGISTER_LOCATION_SOUNDS(engine);
    
    
    REGISTER_COMPONENT(CursorFollower);
    engine.systems.registerUpdete<CursorFollowersSystem>(SystemPriority::INPUT);
    engine.systems.registerAlwaysUpdate<CursorFollowersSystem>();
    
    engine.systems.registerUpdete<SoundCursorSystem>(SystemPriority::AUDIO);
    engine.systems.registerAlwaysUpdate<SoundCursorSystem>();
    
    engine.init(config);

    engine.loadJsonComponent("res/components.json");
    
    engine.world.getSingleComponent<Listener>()->target = engine.world.getNamedEntity("center");
    engine.world.getSingleComponent<Camera>()->target = engine.world.getNamedEntity("center");

    engine.predUpate();
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