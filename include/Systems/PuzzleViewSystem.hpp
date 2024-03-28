#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "Components/BoundingBox.hpp"
#include "InputSystem.hpp"
#include "Scene.hpp"

class PuzzleViewSystem {
public:
    PuzzleViewSystem(Scene& scene);
    void update();

private:
    Scene &scene;

    glm::vec2 prevMousePos;

    void updateExplodedView();
    void updateSolution();
    void updatePuzzleRotation();
    void updateSelectedPieceColor();

    void updatePieceSelection();
    bool mouseHoveringOverPieceView();
    bool getRayBoundingBoxIntersection(glm::vec3 rayStart, glm::vec3 rayDirection, BoundingBox boundingBox, float& intersectionLambda);
    glm::vec3 getBezierPoint( std::vector<glm::vec3> points, float t );

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
