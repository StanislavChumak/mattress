#include "Engine.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include "components/registerComponent.h"
#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"
#include "components/rendering/Animator.h"
#include "components/ui/Cursor.h"

#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/AnimatorSystem.h"
#include "systems/input/CursorFollowersSystem.h"

#define BLOCK_SIZE 32
#define SCALE 3
#define GAME_WIDTH 9 * BLOCK_SIZE
#define GAME_HEIGTH 7 * BLOCK_SIZE


int main(int argc, char **argv)
{
    engine::Core engine;

    engine::Config config;
    config.gameSize = glm::vec2(GAME_WIDTH, GAME_HEIGTH);
    config.nameWindow = "Engine";
    config.pixelScale = SCALE;
    config.executablePath = *argv;
    config.displayCursor = false;

    REGISTER_COMPONENT(Transform);
    REGISTER_COMPONENT(Sprite2D);
    REGISTER_COMPONENT(Animator);
    REGISTER_COMPONENT(CursorFollower);

    engine.systems.registerSystem<RenderSystem>(SystemPriority::RENDERING);
    engine.systems.registerAlwaysRunSystem<RenderSystem>();
    engine.systems.registerSystem<AnimatorSystem>(SystemPriority::ANIMATION);
    engine.systems.registerAlwaysRunSystem<AnimatorSystem>();
    engine.systems.registerSystem<CursorFollowersSystem>(SystemPriority::INPUT);
    engine.systems.registerAlwaysRunSystem<CursorFollowersSystem>();

    engine.inputSystem.mouseButtonSubscribe(0, Input::Action::PRESED, [&](){engine.world.removeEntyti(EntityID(0));});
    
    engine.init(config);

    engine.loadJsonComponent("res/components.json");

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