#include "Engine.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include "components/registerComponent.h"
#include "components/core/Name.h"
#include "components/core/ParentChildren.h"
#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"
#include "components/rendering/Animator.h"
#include "components/ui/Cursor.h"
#include "components/audio/Sound.h"
#include "components/audio/Listener.h"

#include "systems/physics/GlobalTransformSystem.h"
#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/AnimatorSystem.h"
#include "systems/input/CursorFollowersSystem.h"
#include "systems/audio/AudioSystem.h"
#include "systems/audio/LocationAudioSystem.h"

#define BLOCK_SIZE 32
#define SCALE 3
#define GAME_WIDTH 9 * BLOCK_SIZE
#define GAME_HEIGTH 7 * BLOCK_SIZE

class SoundCursorSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

int main(int argc, char **argv)
{
    engine::Core engine;

    engine::Config config;
    config.gameSize = glm::vec2(GAME_WIDTH, GAME_HEIGTH);
    config.nameWindow = "Engine";
    config.pixelScale = SCALE;
    config.executablePath = *argv;
    config.displayCursor = false;

    REGISTER_COMPONENT(Name);
    REGISTER_COMPONENT(Parent);
    REGISTER_COMPONENT(Children);
    REGISTER_COMPONENT(Transform);
    REGISTER_COMPONENT(Sprite2D);
    REGISTER_COMPONENT(Animator);
    REGISTER_COMPONENT(CursorFollower);
    REGISTER_COMPONENT(Sound);

    engine.systems.registerUpdete<GlobalTransformSystem>(SystemPriority::PHYSICS);
    engine.systems.registerAlwaysUpdate<GlobalTransformSystem>();
    engine.systems.registerUpdete<RenderSystem>(SystemPriority::RENDERING);
    engine.systems.registerAlwaysUpdate<RenderSystem>();
    engine.systems.registerUpdete<AnimatorSystem>(SystemPriority::ANIMATION);
    engine.systems.registerAlwaysUpdate<AnimatorSystem>();
    engine.systems.registerUpdete<CursorFollowersSystem>(SystemPriority::INPUT);
    engine.systems.registerAlwaysUpdate<CursorFollowersSystem>();
    engine.systems.registerUpdete<AudioSystem>(SystemPriority::AUDIO);
    engine.systems.registerAlwaysUpdate<AudioSystem>();
    engine.systems.registerUpdete<LocationAudioSystem>(SystemPriority::AUDIO);
    engine.systems.registerAlwaysUpdate<LocationAudioSystem>();
    engine.systems.registerUpdete<SoundCursorSystem>(SystemPriority::AUDIO);
    engine.systems.registerAlwaysUpdate<SoundCursorSystem>();
    
    engine.init(config);

    engine.loadJsonComponent("res/components.json");
    
    engine.world.addSingleComponent(Listener{engine.world.getNamedEntity("cursor")});

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