#include "Engine.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec2.hpp"

#include "jsonManager.h"

#include "components/rendering/Camera.h"
#include "components/audio/Audio.h"
#include "components/ui/Cursor.h"


#include "components/registerComponent.h"
#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"
#include "components/rendering/Animator.h"

#include "systems/rendering/RenderSystem.h"
#include "systems/rendering/AnimatorSystem.h"
#include "systems/input/CursorFollowersSystem.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

using namespace engine;

void windowSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow *window, double xpos, double ypos);

bool loadJsonResource(ECSWorld &world, ResourceManager &resources, std::string pathJsonResource);
bool loadJsonComponent(ECSWorld &world, ResourceManager &resources, std::string pathJsonResource);

Core::Core():inputSystem(world.addSingleComponent(Input{})){}

bool Core::init(const Config& config)
{
    window = &world.addSingleComponent(Window(config.gameSize * config.pixelScale, config.nameWindow.c_str(), config.pixelScale));
    game = &world.addSingleComponent(Game{config.gameSize, config.initState});
    Audio &audio = world.addSingleComponent(Audio{});

    world.addSingleComponent(Cursor{});

    if (!glfwInit())
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed GLFW" << std::endl;
        return false;
#endif
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->poiter = glfwCreateWindow(window->size.x, window->size.y, window->name, nullptr, nullptr);
#ifndef FLAG_RELEASE
    if (!window->poiter)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        const char *description;
        int code = glfwGetError(&description);

        if (description) std::cerr << code << description;
        
        glfwTerminate();
        return false;
    }
#endif
    glfwMakeContextCurrent(window->poiter);

    glfwSetFramebufferSizeCallback(window->poiter, windowSizeCallback);
    glfwSetKeyCallback(window->poiter, keyCallback);
    glfwSetMouseButtonCallback(window->poiter, mouseButtonCallback);
    glfwSetCursorPosCallback(window->poiter, cursorCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
#ifndef FLAG_RELEASE
        std::cerr << "Couidn't load opengl" << std::endl;
        glfwTerminate();
        return false;
#endif
    }

#ifndef FLAG_RELEASE
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif

    audio.init();
#ifndef FLAG_RELEASE
    if(!audio.initialized)
    {
        std::cerr << "Failed to init sound engine" << std::endl;
        glfwTerminate();
        return false;
    }
#endif
    
    glfwSetWindowUserPointer(window->poiter, this);


    if(!config.displayCursor) glfwSetInputMode(window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clearColor[0], config.clearColor[1], config.clearColor[2], config.clearColor[3]);
    
    if(config.depth) glEnable(GL_DEPTH_TEST);

    resources.setExecutablePath(config.executablePath);

    if(!loadJsonResource(world,resources, config.pathJsonResource)) return false;
    if(!loadJsonComponent(world,resources, config.pathJsonComponent)) return false;

    world.addSingleComponent(Camera{});
    
    systems.registerSystem<RenderSystem>(SystemPriority::RENDERING);
    systems.registerAlwaysRunSystem<RenderSystem>();
    systems.registerSystem<AnimatorSystem>(SystemPriority::ANIMATION);
    systems.registerAlwaysRunSystem<AnimatorSystem>();
    systems.registerSystem<CursorFollowersSystem>(SystemPriority::INPUT);
    systems.registerAlwaysRunSystem<CursorFollowersSystem>();


    glfwSetWindowSize(window->poiter, window->size.x, window->size.y);
    windowSizeCallback(window->poiter, window->size.x, window->size.y);
    
    return true;
}

void Core::update(float delta)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, delta, game->currentState);

    glfwSwapBuffers(window->poiter);
}

void Core::shutdown()
{
    resources.clear();
    world.getSingleComponent<Audio>().destroy();
    world.clearComponentStorage<Sprite2D>();
    glfwTerminate();
}

bool loadJsonComponent(ECSWorld &world, ResourceManager &resources, std::string pathJsonComponent)
{
    REGISTER_COMPONENT(Transform);
    REGISTER_COMPONENT(Sprite2D);
    REGISTER_COMPONENT(Animator);
    REGISTER_COMPONENT(CursorFollower);

    std::unordered_map<std::string, std::string> defines;
    std::string JSONstring = preprocessJSON(pathJsonComponent, defines);

    rapidjson::Document document = getDocumentFromJSON(JSONstring);

    auto entitiesIterator = document.FindMember("entities");
    if (entitiesIterator != document.MemberEnd())
    {
        for(const auto &currentEntity : entitiesIterator->value.GetArray())
        {
            EntityID id = world.createEntity();
            for (auto it = currentEntity.MemberBegin(); it != currentEntity.MemberEnd(); ++it)
            {
                const char* name = it->name.GetString();
                const rapidjson::Value& value = it->value;
                typeRegistry[name].addComponentFromJson(id, value, world, resources);
            }
        }
    }

    return true;
}

bool loadJsonResource(ECSWorld &world, ResourceManager &resources, std::string pathJsonResource)
{
    std::unordered_map<std::string, std::string> defines;
    std::string JSONstring = preprocessJSON(pathJsonResource, defines);

    rapidjson::Document document = getDocumentFromJSON(JSONstring);

    auto shadersIterator = document.FindMember("shaders");
    if (shadersIterator != document.MemberEnd())
    {
        for (const auto &currentShader : shadersIterator->value.GetArray())
        {
            const std::string name = currentShader["name"].GetString();
            const std::string pathVertexFile = currentShader["pathVertexFile"].GetString();
            const std::string pathFragmentFile = currentShader["pathFragmentFile"].GetString();
            auto shaderSprite2D = resources.loadShaders(name, pathVertexFile, pathFragmentFile);

#           ifndef FLAG_RELEASE
            if (!shaderSprite2D->is_compiled())
            {
                std::cerr << "Can't find Sheder Program: " << name << std::endl;
                return false;
            }
#           endif
        }
    }

    auto textureAtlasesIterator = document.FindMember("textureAtlases");
    if (textureAtlasesIterator != document.MemberEnd())
    {
        for (const auto &currentTextureAtlases : textureAtlasesIterator->value.GetArray())
        {
            const std::string name = currentTextureAtlases["name"].GetString();
            const std::string filePath = currentTextureAtlases["filePath"].GetString();
            const unsigned int subTextureWidth = currentTextureAtlases["subTextureWidth"].GetUint();
            const unsigned int subTextureHeight = currentTextureAtlases["subTextureHeight"].GetUint();

            const auto subTexturesArray = currentTextureAtlases["subTextures"].GetArray();
            std::vector<std::string> subTextures;
            subTextures.reserve(subTexturesArray.Size());
            for (const auto &currentSubTexture : subTexturesArray)
            {
                subTextures.push_back(currentSubTexture.GetString());
            }
            auto textureAtlase = resources.loadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
#           ifndef FLAG_RELEASE
            if (!textureAtlase)
            {
                std::cerr << "Can't Create Texture Atlase: " << name << std::endl;
                return false;
            }
#           endif
        }
    }

    auto soundIterator = document.FindMember("sounds");
    if (soundIterator != document.MemberEnd())
    {
        Audio &audio = world.getSingleComponent<Audio>();
        for (const auto &currentSound : soundIterator->value.GetArray())
        {
            const std::string name = currentSound["name"].GetString();
            const std::string soundPath = currentSound["soundPath"].GetString();

            unsigned int flag = 0;

            auto currentFlag = currentSound["flag"].GetObject();

            if(currentFlag["NONE"].GetBool()) flag |= Sound::Flag::NONE;
            if(currentFlag["STREAM"].GetBool()) flag |= Sound::Flag::STREAM;
            if(currentFlag["LOOP"].GetBool()) flag |= Sound::Flag::LOOP;
            if(currentFlag["NO_SPATIALIZATION"].GetBool()) flag |= Sound::Flag::NO_SPATIALIZATION;

            auto sound = resources.loadSound(name, &audio.engine, soundPath, flag);
        }
    }

    return true;
}

void windowSizeCallback(GLFWwindow *window, int width, int height)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        ECSWorld &world = core->world;

        glm::uvec2 windowSize = glm::uvec2(width, height);
        Game game = world.getSingleComponent<Game>();
        world.getSingleComponent<Window>().size = windowSize;

        const float aspectRatio = static_cast<float>(game.size.x) / game.size.y;
        unsigned int viewPortWidth = windowSize.x;
        unsigned int viewPortHeight = windowSize.y;
        unsigned int viewPortOffsetLeft = 0;
        unsigned int viewPortOffsetBottom = 0;
        if (static_cast<float>(windowSize.x) / windowSize.y > aspectRatio)
        {
            viewPortWidth = static_cast<unsigned int>(windowSize.y * aspectRatio);
            viewPortOffsetLeft = (windowSize.x - viewPortWidth) / 2;
        }
        else
        {
            viewPortHeight = static_cast<unsigned int>(windowSize.x / aspectRatio);
            viewPortOffsetBottom = (windowSize.y - viewPortHeight) / 2;
        }

        Camera &camera = world.getSingleComponent<Camera>();
        camera.setOffsetViewport(viewPortWidth, viewPortHeight, viewPortOffsetLeft, viewPortOffsetBottom);
        glm::mat4 projection = glm::ortho(0.f, static_cast<float>(game.size.x), 0.f, static_cast<float>(game.size.y), -100.f, 100.f);
        camera.projection = projection;
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->inputSystem.setKey(core->world, key, (Input::Action)action);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->inputSystem.setMouseButton(core->world, button, (Input::Action)action);
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        InputSystem &input = core->inputSystem;
        ECSWorld &world = core->world;
        glm::dvec2 offset(world.getSingleComponent<Camera>().offsetViewport);
        glm::dvec2 pos(xpos, ypos);
        pos -= offset;
        unsigned int gameHeight = world.getSingleComponent<Game>().size.y;
        auto window = world.getSingleComponent<Window>();
        double ratio = (static_cast<double>(window.size.y - offset.y * 2) / window.scale) / gameHeight;
        pos /= window.scale * ratio;
        pos.y = gameHeight - pos.y;

        input.setCursor(world, pos);
    }
}



