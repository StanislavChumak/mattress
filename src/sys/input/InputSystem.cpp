#include "sys/input/InputSystem.h"

#include "comp/single/Cursor.h"

InputSystem::InputSystem(Input &input):input(input){}

void InputSystem::setKey(ECSWorld &world, int key, bool action)
{
    input.keys[key] = action;
    // for(auto func : _subscribersKey[key][action])
    // {
    //     func();
    // }
}

void InputSystem::setMouseButton(ECSWorld &world, int button, bool action)
{
    input.mouseButtons[button] = action;
    // for(auto func : _subscribersMouseButton[button][action])
    // {
    //     func();
    // }
}

void InputSystem::setCursor(ECSWorld &world, glm::dvec2 pos)
{
    Cursor *cursor = world.get_single_comp<Cursor>();
    if(!cursor) return;
    cursor->pos = pos;
    // for(auto func : _subscribersCursor)
    // {
    //     func(cursor);
    // }
}

// void InputSystem::keySubscribe(int key, bool action, std::function<void()> subscriber)
// {
//     _subscribersKey[key][action].push_back(subscriber);
// }

// void InputSystem::mouseButtonSubscribe(int botton, bool action, std::function<void()> subscriber)
// {
//     _subscribersMouseButton[botton][action].push_back(subscriber);
// }

// void InputSystem::cursorSubscribe(std::function<void(glm::dvec2)> subscriber)
// {
//     _subscribersCursor.push_back(subscriber);
// }

void InputSystem::updateLastInput()
{
    for(int i = 0; i < 349; i++)
        input.lastKeys[i] = input.keys[i];
    for(int i = 0; i < 8; i++)
        input.lastMouseButtons[i] = input.mouseButtons[i];
}