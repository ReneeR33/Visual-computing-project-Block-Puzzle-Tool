#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "Systems/UISystem.hpp"

struct Button {
    friend class UISystem;

    Button() 
    : buttonPressEvent(buttonPressEventSignal)
    , prevPressed(false) {}

    glm::vec3 color;
    glm::vec3 hoverColor;
    glm::vec3 pressColor;

    entt::entity background;

    bool pressed;

    entt::sink<entt::sigh<void()>> buttonPressEvent;

private:
    entt::sigh<void()> buttonPressEventSignal;
    bool prevPressed;
};

#endif //BUTTON_HPP