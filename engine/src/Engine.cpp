#include "Engine.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec2.hpp"

#include "components/core/Window.h"
#include "components/core/Game.h"
#include "components/rendering/Camera.h"
#include "components/audio/Audio.h"
#include "components/ui/Cursor.h"

#include "components/registerComponent.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

using namespace engine;

void windowSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorCallback(GLFWwindow *window, double xpos, double ypos);

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

    world.addSingleComponent(Camera{});

    glfwSetWindowSize(window->poiter, window->size.x, window->size.y);
    windowSizeCallback(window->poiter, window->size.x, window->size.y);
    
    return true;
}

void Core::loadJsonComponent(std::string pathJsonComponent)
{
    simdjson::ondemand::document doc = resources.getJSON(pathJsonComponent);
    for(simdjson::ondemand::object entity : getVarJSON<simdjson::ondemand::array>(doc["entities"]))
    {
        EntityID id = world.createEntity();
        for(auto field : entity)
        {
            auto keyResult = field.unescaped_key();
            auto valueResult = field.value().get_object();
#ifndef FLAG_RELEASE
            if (keyResult.error()) {
                std::cerr << "Key read error: " << simdjson::error_message(keyResult.error()) << std::endl;
                continue;
            }
            if (valueResult.error()) {
                std::cerr << "Value read error for component '" << keyResult.value()
                        << "': " << simdjson::error_message(valueResult.error()) << std::endl;
                continue;
            }
#endif      
            std::string_view name = keyResult.value();
            simdjson::ondemand::object obj = valueResult.value();
            typeRegistry[std::string(name)].addComponentFromJson(id, obj, world, resources);
        }
    }
}


void Core::update(float delta)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, delta, game->currentState);

    glfwSwapBuffers(window->poiter);
    resources.collectUnused();
}

void Core::shutdown()
{
    world.clearSets();
    world.clearSingletons();
    glfwTerminate();
}

bool Core::isCloseWindow()
{
    return glfwWindowShouldClose(window->poiter);
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





