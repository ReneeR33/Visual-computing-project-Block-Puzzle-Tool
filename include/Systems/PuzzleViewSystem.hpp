#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "InputSystem.hpp"

class PuzzleViewSystem {
public:
    PuzzleViewSystem(entt::registry& scene);
    void update();

private:
    entt::registry &scene;

    glm::vec2 prevMousePos;

    void updateExplodedView();
    void updatePuzzleRotation();

    bool mouseHoveringOverPieceView();

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
