#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::comp
{
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
}

namespace mtrs::sys
{

class InputSystem : public System<InputSystem>
{
public:
    static comp::KeyButtons *key_buttons;
    static comp::MouseButtons *mouse_buttons;
    static comp::MouseScroll *mouse_scroll;

    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::CLEANUP; }
    static constexpr const char *get_system_name_imp() { return "InputSystem"; }
};

}

#endif
