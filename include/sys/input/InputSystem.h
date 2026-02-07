#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "comp/ECSWorld.h"
#include "comp/single/Input.h"

#include <vector>
#include <functional>

class InputSystem
{
    Input &input;
    // std::vector<std::function<void()>> _subscribersKey[349][3];
    // std::vector<std::function<void()>> _subscribersMouseButton[8][2];
    // std::vector<std::function<void(glm::dvec2)>> _subscribersCursor;
public:
    InputSystem(Input &input);

    void setKey(ECSWorld &world, int key, bool action);
    void setMouseButton(ECSWorld &world, int button, bool action);
    void setCursor(ECSWorld &world, glm::dvec2 cursor);

    // void keySubscribe(int key, bool action, std::function<void()> subscriber);
    // void mouseButtonSubscribe(int botton, bool action, std::function<void()> subscriber);
    // void cursorSubscribe(std::function<void(glm::dvec2)> subscriber);

    void updateLastInput();
};

#endif
