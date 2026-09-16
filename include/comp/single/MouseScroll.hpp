#ifndef MOUSE_SCROLL_HPP
#define MOUSE_SCROLL_HPP

#include "comp/Component.hpp"

#include "glm/vec2.hpp"

#include <vector>

namespace mtrs::comp
{

struct MouseScroll : public Component<MouseScroll>
{
    glm::dvec2 scroll = glm::dvec2{0.0};
    glm::dvec2 last_scroll = glm::dvec2{0.0};
    std::vector<void(*)()> subscribers;

    MouseScroll(const MouseScroll&) = delete;
    MouseScroll &operator=(const MouseScroll&) = delete;
    MouseScroll(MouseScroll&&) = delete;
    MouseScroll &operator=(MouseScroll&&) = delete;

    MouseScroll();
    ~MouseScroll() = default;

    void subscribe(void(*callback)());
    void unsubscribe(void(*callback)());

    static constexpr const char *get_type_name_imp() noexcept { return "MouseScroll"; }
};

}

#endif
