#include "comp/single/Cursor.hpp"

#include <utility>

const glm::mat4 *get_mat4(const void *self) { return static_cast<const glm::mat4*>(self);}

bool update_mat4(void *self)
{
    static const glm::mat4 *last_mat4_ptr = nullptr;
    static glm::mat4 last_mat4_value;
    const auto *m = static_cast<const glm::mat4*>(self);
    if (last_mat4_ptr != m)
    {
        last_mat4_ptr = m;
        last_mat4_value = *m;
        return true;
    }
    if (last_mat4_value != *m)
    {
        last_mat4_value = *m;
        return true;
    }
    return false;
}

mtrs::react::PushNode<glm::mat4> mat4_to_node(glm::mat4 *m)
{
    mtrs::react::PushNode<glm::mat4> node;
    node._get = get_mat4;
    node._update = update_mat4;
    node._self = m;
    return node;
}

namespace mtrs::comp
{

Cursor::Cursor(Camera *camera)
: position([](const glm::vec2 *win_pos, const glm::uvec4 *view, const glm::vec2 *scale)
    {
        return (*win_pos - static_cast<glm::vec2>(*view)) / *scale;
    }, &window_position, &camera->viewport, &camera->point_scale)
, glob_pos([](const glm::vec2 *cursor, const glm::uvec2 *pnts_sz,
    const glm::mat4 *proj, const glm::mat4 *view)
    {
        glm::vec4 clip_pos((float)(cursor->x / pnts_sz->x) * 2.0f - 1.0f,
            1.0f - (float)(cursor->y / pnts_sz->y) * 2.0f, 0.0f, 1.0f);
        return glm::vec2(glm::inverse(*proj * *view) * clip_pos);
    }, &position, &camera->size_in_points, 
            mat4_to_node(&camera->_matrices.proj), mat4_to_node(&camera->_matrices.view))
{
    window_position.add_observer(&position);
    camera->viewport.add_observer(&position);
    camera->point_scale.add_observer(&position);

    position.add_observer(&glob_pos);
    camera->size_in_points.add_observer(&glob_pos);
}

void Cursor::subscribe(void(*callback)())
{
    subscribers.push_back(callback);
}

void Cursor::unsubscribe(void(*callback)())
{
    for(auto iter = subscribers.begin(); iter != subscribers.end(); iter++)
    {
        if(*iter == callback)
        {
            subscribers.erase(iter);
        }
    }
}

}