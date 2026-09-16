#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include "comp/ECSWorld.hpp"
#include "sys/SystemManager.hpp"
#include "res/ResourceManager.hpp"

#include <chrono>

namespace mtrs
{

namespace comp
{
    struct Window;
    struct Camera;
    struct States;
    struct Cursor;
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
}

namespace engine
{
    struct Config
    {
        // Base
        std::string executable_path;
        std::string packs_path = "./";
        std::string scenes_path = "./";
        std::string name_window = "";
        std::string start_scene = "";
        uint64_t scenes_cache_limit = 5;
        uint64_t packs_cache_limit = 5;
        bool blend = true;
        
        // Window
        glm::uvec2 size_in_points;
        unsigned int start_point_size = 1;
        std::vector<const char*> icon_window;
        float clear_color[4] = {0.2f, 0.0f, 0.3f, 1.f};
        bool display_cursor = true;

        // Camera
        unsigned int max_fps = 60;
        bool fixed_horizontal = true;
        bool fixed_vertical = true;

        // States
        std::string init_state;

        // Audio
        int saund_location_scale = 10;
    };

    void window_size_callback(GLFWwindow *window, int width, int height);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void cursor_callback(GLFWwindow *window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);

    class Core 
    {
        comp::Window *_window;
        comp::Camera *_camera;
        comp::States *_states;
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

        Core(const Config& config);
        bool is_init() { return _is_init; }
        void pre_update();
        void update();
        void shutdown();

        bool is_close_window();
    };
}

}

#endif
