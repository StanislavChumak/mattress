#include "Engine.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/Render.hpp"
#include "comp/single/GlyphDecoder.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"

#include "sys/core/InputSystem.hpp"
#include "sys/core/InputSybscrubersSystem.hpp"
#include "sys/rendering/RenderSystem.hpp"
#include "sys/rendering/SpriteSubmitSystem.hpp"
#include "sys/rendering/SpriteMapSubmitSystem.hpp"
#include "sys/rendering/TextSubmitSystem.hpp"
#include "sys/rendering/CameraSystem.hpp"

#include "util/fun/msg/mtrs_message.hpp"

#include <thread>

namespace mtrs::engine
{

Core::Core(const Config& config)
: resources(config.executable_path + config.pack_dir, config.packs_cache_limit)
, world(config.executable_path + config.scene_dir, config.scenes_cache_limit)
{
    world.set_resources(&resources);
#ifndef FLAG_RELEASE
    if(!config.fixed_horizontal && !config.fixed_vertical)
    {
        msg::mtrs_error("Only one side can be non-fixed, correct the configuration");
    }
#endif

    if (!glfwInit())
    {
#ifndef FLAG_RELEASE
        msg::mtrs_error("Failed GLFW init");
        _is_init = false;
#endif
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = world.init_single_comp<comp::Window>(config.size_in_points * config.start_point_size, config.name_window.c_str());
    const glm::uvec2 &window_size = _window->size.get();

    _window->poiter = glfwCreateWindow(window_size.x, window_size.y, _window->name, nullptr, nullptr);

#ifndef FLAG_RELEASE
    if (!_window->poiter)
    {
        const char *description;
        int code = glfwGetError(&description);

        if (description)
        {
            msg::mtrs_error("Failed to create GLFW window: ", code);
        }
        
        glfwTerminate();
        _is_init = false;
    }
#endif
    glfwMakeContextCurrent(_window->poiter);

    glfwSetWindowUserPointer(_window->poiter, this);

    glfwSetFramebufferSizeCallback(_window->poiter, window_size_callback);
    glfwSetKeyCallback(_window->poiter, key_callback);
    glfwSetMouseButtonCallback(_window->poiter, mouse_button_callback);
    glfwSetScrollCallback(_window->poiter, scroll_callback);
    glfwSetCursorPosCallback(_window->poiter, cursor_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
#ifndef FLAG_RELEASE
        msg::mtrs_error("Couidn't load opengl");
        glfwTerminate();
        _is_init = false;
#endif
    }

    glfwSwapInterval(0);

    _states = world.init_single_comp<comp::States>(config.init_state);
    _camera = world.init_single_comp<comp::Camera>(_window, glm::bvec2{config.fixed_horizontal,
        config.fixed_vertical}, config.size_in_points);
    _cursor = world.init_single_comp<comp::Cursor>(_camera);
    _keyboard = world.init_single_comp<comp::KeyButtons>();
    _mouse = world.init_single_comp<comp::MouseButtons>();
    _scroll = world.init_single_comp<comp::MouseScroll>();

#ifndef FLAG_RELEASE
    msg::mtrs_info("Renderer: ", glGetString(GL_RENDERER));
    msg::mtrs_info("OpenGL version: ", glGetString(GL_VERSION));
    msg::mtrs_info("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

    comp::Audio *audio = world.init_single_comp<comp::Audio>();
    if(audio)
    {
        audio->sound_scale = config.saund_location_scale;
#ifndef FLAG_RELEASE
        if(!audio->is_init)
        {
            msg::mtrs_error("Failed to init sound engine");
            glfwTerminate();
            _is_init = false;
        }
#endif
    }
    
    if(!config.display_cursor) glfwSetInputMode(_window->poiter, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glClearColor(config.clear_color[0], config.clear_color[1], config.clear_color[2], config.clear_color[3]);
    
    if(config.blend) { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);}

    world.init_single_comp<comp::GlyphDecoder>(resources);

    comp::Render *render = world.init_single_comp<comp::Render>();
    sys::RenderSystem::render = render;
    sys::SpriteSubmitSystem::render = render;
    sys::SpriteMapSubmitSystem::render = render;
    sys::TextSubmitSystem::render = render;
    
    sys::InputSystem::key_buttons = _keyboard;
    sys::InputSystem::mouse_buttons = _mouse;
    sys::InputSystem::mouse_scroll = _scroll;

    sys::InputSybscrubersSystem::callbacks = &_input_callbacks;

    sys::CameraSystem::camera = _camera;

    _window->set_icon(config.icon_window.data(), config.icon_window.size());

    _time_frame = std::chrono::duration<double>(1.0 / config.max_fps);

#ifndef FLAG_RELEASE
    if(config.start_scene == "")
    {
        msg::mtrs_warning("You need to select the starting scene ",
            "in the configuration via the \"start_scene\" parameter");
    }
    else
#endif
    {
        world.load_scene(math::hash64(config.start_scene), math::hash64("start_scene"));
    }
    
    _is_init = true;
}

void Core::pre_update()
{
    const glm::uvec2 &window_size = _window->size.get();
    glfwSetWindowSize(_window->poiter, window_size.x, window_size.y);
    window_size_callback(_window->poiter, window_size.x, window_size.y);

    _end = std::chrono::steady_clock::now();
}

void Core::update()
{
    _start = _end;

    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.update(world, _delta.count());

    resources.update(_delta.count());
    world.update(_delta.count());

    glfwSwapBuffers(_window->poiter);

    _end = std::chrono::steady_clock::now();
    _delta = _end - _start;
    if(_delta < _time_frame)
    {
        std::this_thread::sleep_for(_time_frame - _delta);
        _end = std::chrono::steady_clock::now();
        _delta = _end - _start;
    }
}

void Core::shutdown()
{
    world.clear_all();
    resources.clear_all();
    glfwTerminate();
}

bool Core::is_close_window()
{
    return glfwWindowShouldClose(_window->poiter);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    Core *core = static_cast<Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_window->size.set({width, height});
        if(!core->_camera->fixed_sides.y)
        {
            core->_camera->size_in_points.set_field(&glm::uvec2::y,
                height / (width / (double)core->_camera->size_in_points->x));
        }
        else if(!core->_camera->fixed_sides.x)
        {
            core->_camera->size_in_points.set_field(&glm::uvec2::x,
                width / (height / (double)core->_camera->size_in_points->y));
        }
        core->_camera->viewport.update();
        core->_camera->update_proj_matrix();
        core->_input_callbacks.reserve(core->_input_callbacks.size() +
            core->_window->size_subscribers.size());
        core->_input_callbacks.insert(core->_input_callbacks.end(),
            core->_window->size_subscribers.begin(), core->_window->size_subscribers.end());
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if(core)
    {
        core->_keyboard->keys[key] = action;
        
        if(action != 2)
        {
            uint32_t key_id = (static_cast<uint32_t>(key) << 1) | action;
            auto it = core->_keyboard->subscribers.find(key_id);
            if (it != core->_keyboard->subscribers.end())
            {
                core->_input_callbacks.reserve(core->_input_callbacks.size() +
                    it->second.size());
                core->_input_callbacks.insert(core->_input_callbacks.end(),
                    it->second.begin(), it->second.end());
            }
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_mouse->buttons[button] = action;
        
        if(action != 2)
        {
            uint32_t button_id = (static_cast<uint32_t>(button) << 1) | action;
            auto it = core->_mouse->subscribers.find(button_id);
            if (it != core->_mouse->subscribers.end())
            {
                core->_input_callbacks.reserve(core->_input_callbacks.size() +
                    it->second.size());
                core->_input_callbacks.insert(core->_input_callbacks.end(),
                    it->second.begin(), it->second.end());
            }
        }
    }
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_scroll->scroll += glm::dvec2{x_offset, y_offset};
        core->_input_callbacks.reserve(core->_input_callbacks.size() +
            core->_scroll->subscribers.size());
        core->_input_callbacks.insert(core->_input_callbacks.end(),
            core->_scroll->subscribers.begin(), core->_scroll->subscribers.end());
    }
}

void cursor_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    mtrs::engine::Core *core = static_cast<mtrs::engine::Core*>(glfwGetWindowUserPointer(window));
    if (core)
    {
        core->_cursor->window_position.set({x_pos, y_pos});
        core->_cursor->position.update();
        core->_input_callbacks.reserve(core->_input_callbacks.size() +
            core->_cursor->subscribers.size());
        core->_input_callbacks.insert(core->_input_callbacks.end(),
            core->_cursor->subscribers.begin(), core->_cursor->subscribers.end());
    }
}

}