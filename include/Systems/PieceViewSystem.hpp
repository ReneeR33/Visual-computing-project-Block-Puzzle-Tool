#ifndef PIECE_VIEW_SYSTEM
#define PIECE_VIEW_SYSTEM

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "InputSystem.hpp"

class PieceViewSystem {
public:
    PieceViewSystem(entt::registry& scene);
    void update();

private:
    entt::registry& scene;

    glm::vec2 prevMousePos;

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PIECE_VIEW_SYSTEM