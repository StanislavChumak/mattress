#include "Engine.h"

#include <glm/gtc/matrix_transform.hpp>

#include "components/core/Window.h"
#include "components/core/States.h"
#include "components/rendering/Camera.h"
#include "components/audio/Audio.h"
#include "components/ui/Cursor.h"

#include "components/registerComponent.h"

#include "resources/RenderContext.h"
#include "systems/rendering/SpriteRenderSystem.h"

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
    window = &world.addSingleComponent(Window(config.pixelSize * config.pixelScale, config.nameWindow.c_str(), config.pixelScale));
    states = &world.addSingleComponent(States{config.initState});

    world.addSingleComponent(Cursor{});

    if (!glfwInit())
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed GLFW" << std::endl;
        return false;
#endif
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    Camera &camera = world.addSingleComponent(Camera(config.pixelSize));

    Audio *audio = world.getSingleComponent<Audio>();
    if(audio)
    {
        audio->soundScale = config.SaundLocationScale;
        audio->init();
#ifndef FLAG_RELEASE
        if(!audio->initialized)
        {
            std::cerr << "Failed to init sound engine" << std::endl;
            glfwTerminate();
            return false;
        }
#endif
    }
    
    glfwSetWindowUserPointer(window->poiter, this);

    if(!config.displayCursor) glfwSetInputMode(window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clearColor[0], config.clearColor[1], config.clearColor[2], config.clearColor[3]);
    
    if(config.depth) glEnable(GL_DEPTH_TEST);

    glfwSetWindowSize(window->poiter, window->size.x, window->size.y);
    windowSizeCallback(window->poiter, window->size.x, window->size.y);

    std::shared_ptr<RenderContext> context = std::make_shared<RenderContext>();
    resources.getCache<RenderContext>()["context"] = context;
    SpriteRenderSystem::context = std::move(context);
    
    return true;
}

void Core::loadJsonComponent(std::string pathJsonComponent)
{
    std::shared_ptr<simdjson::padded_string> json = resources.getJSON(pathJsonComponent);
    simdjson::ondemand::parser parser;
    simdjson::ondemand::document doc = parser.iterate(*json);
    for(simdjson::ondemand::object entity : getVarJSON<simdjson::ondemand::array>(doc["entities"]))
    {
        EntityID id = world.createEntity();
        for(auto field : entity)
        {
            std::string_view name = getResultJSON<std::string_view>(field.unescaped_key());
            simdjson::ondemand::object obj = getVarJSON<simdjson::ondemand::object>(field.value());
            typeRegistry[std::string(name)].addComponentFromJson(id, obj, world, resources);
        }
    }
}

void Core::predUpate()
{
    windowSizeCallback(window->poiter, window->size.x, window->size.y);
}

void Core::update(float delta)
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, delta, states->currentSystemState);

    inputSystem.updateLastInput();
    world.removeMarked();

    glfwSwapBuffers(window->poiter);
    resources.garbageCollector();
}

void Core::shutdown()
{
    SpriteRenderSystem::context.reset();
    world.clearSets();
    world.clearSingletons();
    resources.garbageCollector();
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
        glm::uvec2 windowSize = glm::uvec2(width, height);
        core->world.getSingleComponent<Window>()->size = windowSize;
        Camera *camera = core->world.getSingleComponent<Camera>();

        const float aspectRatio = static_cast<float>(camera->pixelSize.x) / camera->pixelSize.y;
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

        camera->setOffsetViewport(viewPortWidth, viewPortHeight, viewPortOffsetLeft, viewPortOffsetBottom);
        camera->updateProjectionMatrix();
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->inputSystem.setKey(core->world, key, action);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
        core->inputSystem.setMouseButton(core->world, button, action);
}

void cursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        Camera *camera = core->world.getSingleComponent<Camera>();
        Window *window = core->world.getSingleComponent<Window>();
        glm::dvec2 offset(camera->offsetViewport);
        glm::dvec2 pos(xpos, ypos);
        pos -= offset;
        unsigned int pixelHeight = camera->pixelSize.y;
        double ratio = (static_cast<double>(window->size.y - offset.y * 2) / window->scale) / pixelHeight;
        pos /= window->scale * ratio;
        pos.y = pixelHeight - pos.y;

        core->inputSystem.setCursor(core->world, pos);
    }
}





