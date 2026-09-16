#ifndef WINDOW_HPP
#define WINDOW_HPP

#ifndef SCRIPT
    #include "glad/glad.h"
    #include "GLFW/glfw3.h"
#else
    using GLFWwindow = void;
    using GLFWmonitor = void;
    using GLFWvidmode = void;
#endif


#include "comp/Component.hpp"

#include "util/type/react/ReactiveStruct.hpp"

#include "glm/vec2.hpp"

#include <vector>

namespace mtrs::comp
{

struct Window : public Component<Window>
{
    GLFWwindow *poiter = nullptr;

private:
    GLFWmonitor* _monitor = nullptr;
    const GLFWvidmode* _mode = nullptr;

    glm::ivec2 _buffer_position;
    glm::ivec2 _buffer_size;
    
public:
    const char *name;
    react::ReactiveStruct<glm::uvec2, 2> size;
    std::vector<void(*)()> size_subscribers;

    Window() = delete;
    Window(const Window &) = delete;
    Window &operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window &operator=(Window&&) = delete;
    
    Window(const glm::uvec2 &size, const char *name);
    ~Window() = default;

    void set_icon(const char *const*paths, uint64_t count);
    void set_position(glm::uvec2 position);
    void set_full_screen(bool is_full_screen);

    glm::ivec2 get_position();

    void subscribe_to_size(void(*callback)());
    void unsubscribe_to_size(void(*callback)());

    static constexpr const char *get_type_name_imp() noexcept { return "Window"; }
};

}

#endif
