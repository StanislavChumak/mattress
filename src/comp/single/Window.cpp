#include "comp/single/Window.hpp"

#define  STBI_ONLY_PNG
#define  STBI_ONLY_JPEG
#include "stb_image.h"

#include "util/fun/msg/mtrs_message.hpp"

#include <string>

namespace mtrs::comp
{

Window::Window(const std::string *executable_path)
: size({ENGINE_WINDOW_SIZE_X * ENGINE_START_POINT_SIZE, ENGINE_WINDOW_SIZE_Y * ENGINE_START_POINT_SIZE})
, name(ENGINE_WINDOW_NAME)
, _monitor(glfwGetPrimaryMonitor())
, _mode(glfwGetVideoMode(_monitor))
, _buffer_position(0)
, _executable_path(executable_path)
{

}

void Window::set_icon(const char *const *paths, uint64_t count)
{
    GLFWimage icon_window[count];

    int channels;
    std::string path;
    for(size_t i = 0; i < count; i++)
    {
        path = *_executable_path + std::string(paths[i]);
        if(path.substr(path.length() - 4) == ".png")
        {
            channels = 4;
        }
        else
        {
            channels = 3;
        }
        icon_window[i].pixels = stbi_load(path.c_str(),
            &icon_window[i].width, &icon_window[i].height, &channels, 0);
#ifndef FLAG_RELEASE
        if(!icon_window[i].pixels)
        {
            msg::mtrs_error("Failed to load icon: ", paths[i]);
            return;
        }
#endif
    }

    glfwSetWindowIcon(poiter, count, icon_window);
    for(auto &icon : icon_window)
    {
        stbi_image_free(icon.pixels);
    }
}

void Window::set_position(glm::uvec2 position)
{
    glfwSetWindowPos(poiter, position.x, position.y);
}

void Window::set_full_screen(bool is_full_screen)
{
    if(is_full_screen)
    {
        glfwGetWindowPos(poiter, &_buffer_position.x, &_buffer_position.y);
        glfwGetWindowSize(poiter, &_buffer_size.x, &_buffer_size.y);
        glfwSetWindowMonitor(poiter, _monitor, 0, 0, _mode->width, _mode->height, _mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(poiter, NULL, _buffer_position.x, _buffer_position.y,
            _buffer_size.x, _buffer_size.y, GLFW_DONT_CARE);
    }
}

glm::ivec2 Window::get_position()
{
    glm::ivec2 pos;
    glfwGetWindowPos(poiter, &pos.x, &pos.y);
    return pos;
}

void Window::subscribe_to_size(void(*callback)())
{
    size_subscribers.push_back(callback);
}

void Window::unsubscribe_to_size(void(*callback)())
{
    for(auto iter = size_subscribers.begin(); iter != size_subscribers.end(); iter++)
    {
        if(*iter == callback)
        {
            size_subscribers.erase(iter);
            return;
        }
    }
}

}