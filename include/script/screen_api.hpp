#ifndef SCREEN_API_HPP
#define SCREEN_API_HPP

#include "engine_api.hpp"
#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"

typedef mtrs::comp::Window Window;
typedef mtrs::comp::Camera Camera;

extern mtrs::EngineAPI *api;

void Window::set_icon(const char *const *paths, uint64_t count)
{
    api->window_set_icon(this, paths, count);
}

void Window::set_position(glm::uvec2 position)
{
    api->window_set_position(this, position);
}

void Window::set_full_screen(bool is_full_screen)
{
    api->window_set_full_screen(this, is_full_screen);
}

glm::ivec2 Window::get_position()
{
    return api->window_get_position(this);
}

void Window::subscribe_to_size(void (*callback)())
{
    api->window_subscribe_to_size(this, callback);
}

void Window::unsubscribe_to_size(void (*callback)())
{
    api->window_unsubscribe_to_size(this, callback);
}

void Camera::update_UBO()
{
    api->camera_update_UBO(this);
}

void Camera::update_proj_matrix()
{
    api->camera_update_proj_matrix(this);
}

void Camera::update_view_matrix()
{
    api->camera_update_view_matrix(this);
}

#endif
