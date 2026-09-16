#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "comp/Component.hpp"

#include "comp/core/Transform.hpp"

namespace mtrs::comp
{

struct Listener : public Component<Listener>
{
    Transform *target;

    Listener(const Listener&) = delete;
    Listener &operator=(const Listener&) = delete;
    Listener(Listener&&) = delete;
    Listener &operator=(Listener&&) = delete;
    
    Listener();
    ~Listener() = default;

    static constexpr const char *get_type_name_imp() noexcept { return "Listener"; }
};

}

#endif
