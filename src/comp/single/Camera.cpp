#include "comp/single/Camera.hpp"

#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mtrs::comp
{

#if !ENGINE_FIXED_HORIZONTAL && !ENGINE_FIXED_VERTICAL
    #error "Only one side can be non-fixed (ENGINE_FIXED_HORIZONTAL / ENGINE_FIXED_VERTICAL), correct the configuration"
#endif

Camera::Camera(Window *window)
: size_in_points({ENGINE_WINDOW_SIZE_X, ENGINE_WINDOW_SIZE_Y})
, viewport([](const glm::uvec2 *win_sz, const glm::uvec2 *sz_pnts)
    {
        double ratio = static_cast<double>(sz_pnts->x) / sz_pnts->y;
        if (static_cast<double>(win_sz->x) / win_sz->y > ratio)
        {
            uint32_t width = win_sz->y * ratio;
            uint32_t x = (win_sz->x - width) >> 1;
            glViewport(x, 0, width, win_sz->y);
            return glm::uvec4{x, 0, width, win_sz->y};
        }
        else
        {
            uint32_t height = win_sz->x / ratio;
            uint32_t y = (win_sz->y - height) >> 1;
            glViewport(0, y, win_sz->x, height);
            return glm::uvec4{0, y, win_sz->x, height};
        }
    }, &window->size, &this->size_in_points)
, point_scale([](const glm::uvec2 *win_sz, const glm::uvec4 *view, const glm::uvec2 *sz_pnts)
    {
        return glm::vec2{view->z, view->w} / static_cast<glm::vec2>(*sz_pnts);
    }, &window->size, &viewport, &this->size_in_points)
{
    window->size.add_observer(&viewport);
    this->size_in_points.add_observer(&viewport);
    window->size.add_observer(&point_scale);
    this->size_in_points.add_observer(&point_scale);

    if constexpr (!(ENGINE_FIXED_HORIZONTAL && ENGINE_FIXED_VERTICAL))
    {
        viewport.set_calculator([](const glm::uvec2 *win_sz, const glm::uvec2 *sz_pnts)
        {
            glViewport(0, 0, win_sz->x, win_sz->y);
            return glm::uvec4{0, 0, win_sz->x, win_sz->y};
        });
    }

    _UBO.init(GL_UNIFORM_BUFFER, &_matrices, sizeof(_matrices), GL_DYNAMIC_DRAW);
    _UBO.bind_base(0);

    _ui_UBO.init(GL_UNIFORM_BUFFER, &_ui_projection, sizeof(_ui_projection), GL_DYNAMIC_DRAW);
    _ui_UBO.bind_base(1);

    _matrices.view = glm::mat4{1.f};

    update_proj_matrix();
}

void Camera::update_UBO()
{
    _UBO.update(&_matrices, sizeof(_matrices), 0);
    _ui_UBO.update(&_ui_projection, sizeof(_ui_projection), 0);
}

void Camera::update_proj_matrix()
{
    const glm::uvec2 &pnts_sz = size_in_points.get();
    _matrices.proj = glm::ortho(pnts_sz.x * -0.5f, pnts_sz.x * 0.5f,
        pnts_sz.y * -0.5f, pnts_sz.y * 0.5f, -100.f, 100.f);
    _ui_projection = glm::ortho(0.f, (float)pnts_sz.x,
        (float)pnts_sz.y, 0.f, -100.f, 100.f);
    update_UBO();
}

void Camera::update_view_matrix()
{
    _matrices.view = glm::inverse(*target_matrix);
    _UBO.update(&_matrices, sizeof(_matrices), 0);
}

}