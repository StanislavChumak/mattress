#ifndef MOUSE_BUTTONS_HPP
#define MOUSE_BUTTONS_HPP

#include "comp/Component.hpp"

#include <vector>
#include <unordered_map>

namespace mtrs::comp
{

struct MouseButtons : public Component<MouseButtons>
{
    bool buttons[8];
    bool last_buttons[8];
    std::unordered_map<uint32_t, std::vector<void(*)()>> subscribers;

    MouseButtons(const MouseButtons&) = delete;
    MouseButtons &operator=(const MouseButtons&) = delete;
    MouseButtons(MouseButtons&&) = delete;
    MouseButtons &operator=(MouseButtons&&) = delete;
    
    MouseButtons();
    ~MouseButtons() = default;

    void subscribe(int button, bool action, void(*callback)());
    void unsubscribe(int button, bool action, void(*callback)());

    static constexpr const char *get_type_name_imp() noexcept { return "MouseButtons"; }
};

#define MTRS_MOUSE_PRESS 0
#define MTRS_MOUSE_RELEASE 1

}
#endif