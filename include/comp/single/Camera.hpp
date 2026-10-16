#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "comp/Component.hpp"

#ifndef SCRIPT
    #include "res/render/BufferObject.hpp"
#else
    namespace mtrs::res
    {
        typedef uint64_t BufferObject;
    }
#endif
#include "comp/single/Window.hpp"

#include "util/type/react/ReactiveStruct.hpp"

namespace mtrs::comp
{

struct Camera : public Component<Camera>
{
    friend struct Cursor;
private:
    res::BufferObject _UBO, _ui_UBO;
    struct {
        glm::mat4 proj;
        glm::mat4 view;
    } _matrices;
    glm::mat4 _ui_projection;

public:
    react::ReactiveStruct<glm::uvec2, 3> size_in_points;
    react::ReactiveStruct<glm::uvec4, 1, glm::uvec2, glm::uvec2> viewport;
    react::ReactiveStruct<glm::vec2, 1, glm::uvec2, glm::uvec4, glm::uvec2> point_scale;

    const glm::mat4 *target_matrix = nullptr;

    Camera() = delete;
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera &operator=(Camera&&) = delete;
    
    Camera(Window *window);
    ~Camera() = default;

    void update_UBO();

    void update_proj_matrix();

    void update_view_matrix();

    static constexpr const char *get_type_name_imp() noexcept { return "Camera"; }
};

}

#endif
