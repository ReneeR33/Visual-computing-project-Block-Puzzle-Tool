#ifndef PIECE_VIEW_SYSTEM
#define PIECE_VIEW_SYSTEM

#include <entt/entt.hpp>
#include "InputSystem.hpp"

class PieceViewSystem {
public:
    PieceViewSystem(entt::registry& scene);
    void update();

private:
    entt::registry& scene;

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
};

#endif //PIECE_VIEW_SYSTEM