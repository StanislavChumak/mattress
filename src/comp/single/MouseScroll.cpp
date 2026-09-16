#include "comp/single/MouseScroll.hpp"

namespace mtrs::comp
{

MouseScroll::MouseScroll()
{
}

void MouseScroll::subscribe(void(*callback)())
{
    subscribers.push_back(callback);
}

void MouseScroll::unsubscribe(void(*callback)())
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