#include "InputSystem.h"

#include "components/ui/Cursor.h"

InputSystem::InputSystem(Input &input):input(input){}

void InputSystem::setKey(ECSWorld &world, int key, Input::Action action)
{
    input.keys[key] = action;
    for(auto func : _subscribersKey[key][action])
    {
        func();
    }
}

void InputSystem::setMouseButton(ECSWorld &world, int button, Input::Action action)
{
    input.mouseButtons[button] = action;
    for(auto func : _subscribersMouseButton[button][action])
    {
        func();
    }
}

void InputSystem::setCursor(ECSWorld &world, glm::dvec2 cursor)
{
    world.getSingleComponent<Cursor>().pos = cursor;
    for(auto func : _subscribersCursor)
    {
        func(cursor);
    }
}

void InputSystem::keySubscribe(int key, Input::Action action, std::function<void()> subscriber)
{
    _subscribersKey[key][action].push_back(subscriber);
}

void InputSystem::mouseButtonSubscribe(int botton, Input::Action action, std::function<void()> subscriber)
{
    _subscribersMouseButton[botton][action].push_back(subscriber);
}

void InputSystem::cursorSubscribe(std::function<void(glm::dvec2)> subscriber)
{
    _subscribersCursor.push_back(subscriber);
}