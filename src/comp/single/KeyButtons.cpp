#include "comp/single/KeyButtons.hpp"

#include <cstring>

namespace mtrs::comp
{

KeyButtons::KeyButtons()
{
    std::memset(keys, 0, sizeof(keys));
}

void KeyButtons::subscribe(int key, bool action, void(*callback)())
{
    if (key < 0 || key >= 348) return;
    if (action < 0 || action > 1) return;

    subscribers[(key << 1) | action].push_back(callback);
}

void KeyButtons::unsubscribe(int key, bool action, void(*callback)())
{
    auto it = subscribers.find((key << 1) | action);
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