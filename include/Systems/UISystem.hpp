#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <entt/entt.hpp>

#include "InputSystem.hpp"

class UISystem {
public:
    UISystem(entt::registry& scene);

    void update();

private:
    entt::registry& scene;

    void updateScrollView();
    void updateButtons();

    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //UI_SYSTEM_HPP