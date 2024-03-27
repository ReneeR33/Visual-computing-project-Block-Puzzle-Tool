#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "Components/BoundingBox.hpp"
#include "InputSystem.hpp"

class PuzzleViewSystem {
public:
    PuzzleViewSystem(entt::registry& scene);
    void update();

private:
    entt::registry &scene;

    glm::vec2 prevMousePos;

    void updateSolution();
    void updateExplodedView();
    void updatePuzzleRotation();
    void updateSelectedPieceColor();

    void updatePieceSelection();
    bool mouseHoveringOverPieceView();
    bool getRayBoundingBoxIntersection(glm::vec3 rayStart, glm::vec3 rayDirection, BoundingBox boundingBox, float& intersectionLambda);

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
