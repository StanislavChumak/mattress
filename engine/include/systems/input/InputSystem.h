#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../../ECSWorld.h"
#include "components/core/Input.h"

#include <vector>
#include <functional>

class InputSystem
{
    Input &input;
    std::vector<std::function<void()>> _subscribersKey[349][3];
    std::vector<std::function<void()>> _subscribersMouseButton[8][2];
    std::vector<std::function<void(glm::dvec2)>> _subscribersCursor;
public:
    InputSystem(Input &input);

    void setKey(ECSWorld &world, int key, Input::Action action);
    void setMouseButton(ECSWorld &world, int button, Input::Action action);
    void setCursor(ECSWorld &world, glm::dvec2 cursor);

    void keySubscribe(int key, Input::Action action, std::function<void()> subscriber);
    void mouseButtonSubscribe(int botton, Input::Action action, std::function<void()> subscriber);
    void cursorSubscribe(std::function<void(glm::dvec2)> subscriber);
};

#endif
