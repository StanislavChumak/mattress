#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include "comp/ECSWorld.hpp"
#include "sys/SystemManager.hpp"
#include "res/ResourceManager.hpp"

#include <chrono>
#include <string>

namespace mtrs
{

namespace comp
{
    struct Window;
    struct Camera;
    struct Cursor;
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
}

namespace engine
{
    void window_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

    class Core 
    {
        comp::Window *_window;
        comp::Camera *_camera;
        comp::Cursor *_cursor;
        comp::KeyButtons *_keyboard;
        comp::MouseButtons *_mouse;
        comp::MouseScroll *_scroll;

        std::vector<void(*)()> _input_callbacks;

        bool _is_init;

        std::chrono::duration<double> _time_frame;
        std::chrono::duration<double> _delta;
        std::chrono::steady_clock::time_point _start, _end;

        friend void window_size_callback(GLFWwindow *window, int width, int height);
        friend void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        friend void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
        friend void cursor_callback(GLFWwindow *window, double xpos, double ypos);

    public:
        res::ResourceManager resources;
        comp::ECSWorld world;
        sys::SystemManager systems;

        Core(const std::string& executable_path);
        bool is_init() { return _is_init; }
        void pre_update();
        void update();
        void shutdown();

        bool is_close_window();
    };
}

}

#endif
