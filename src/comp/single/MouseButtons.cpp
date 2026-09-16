#include "comp/single/MouseButtons.hpp"

#include <cstring>

namespace mtrs::comp
{

MouseButtons::MouseButtons()
{
    std::memset(buttons, 0, sizeof(buttons));
}

void MouseButtons::subscribe(int button, bool action, void(*callback)())
{
    if (button < 0 || button >= 8) return;
    if (action < 0 || action > 1) return;
    
    subscribers[(button << 1) | action].push_back(callback);
}

void MouseButtons::unsubscribe(int button, bool action, void(*callback)())
{
    if (button < 0 || button >= 8) return;
    if (action < 0 || action > 1) return;
    
    auto it = subscribers.find((button << 1) | action);
    if (it != subscribers.end())
    {
        auto &vec = it->second;
        for(auto iter = vec.begin(); iter != vec.end(); iter++)
        {
            if(*iter == callback)
            {
                vec.erase(iter);
            }
        }
    }
}

}