#include "Systems/PuzzleViewSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <limits>
#include <cmath>

#include "entity.hpp"
#include "Components/ExplodedView.hpp"
#include "Components/Transform.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Camera.hpp"
#include "Components/Puzzle.hpp"
#include "Components/PiecesView.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/Transform2D.hpp"
#include "Components/BoundingBox.hpp"
#include "Components/Children.hpp"
#include "Components/Material.hpp"
#include "Components/Solution.hpp"

#include <iostream>

//TODO: move this somewhere else
#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 950

#define ZOOM_SPEED 5.0f
#define ROTATE_SPEED 0.25f

PuzzleViewSystem::PuzzleViewSystem(Scene &scene)
    : scene(scene)
    , prevMousePos(glm::vec2(0.0f))
    , pieceSelected(false) {
    InputSystem::event<InputSystem::ScrollEvent>().connect<&PuzzleViewSystem::scrollCallback>(*this);
    InputSystem::event<InputSystem::MouseButtonCallBackEvent>().connect<&PuzzleViewSystem::mouseButtonCallback>(*this);
}

void PuzzleViewSystem::update() {
    updateExplodedView();
    updateSolution();
    updatePuzzleRotation();
    updateSelectedPieceColor();
    updatePieceTransparency();
}

void PuzzleViewSystem::updateExplodedView() {
    auto explodedViewView = scene.registry.view<ExplodedView>();
    if (explodedViewView.empty()) {
        return;
    }
    auto& explodedView = scene.registry.get<ExplodedView>(explodedViewView.front());

    auto piecesView = scene.registry.view<PuzzlePiece, Transform>();
    PuzzlePiece* centerPiece = nullptr;
    for (auto [entity, piece, transform] : piecesView.each()) {
        if (centerPiece == nullptr || glm::length(piece.initialPosition) < glm::length(centerPiece->initialPosition)) {
            centerPiece = &piece;
        }
    }

    if (centerPiece == nullptr) {
        return;
    }

    auto piecesViewTransform = scene.registry.view<PuzzlePiece, Transform>();
    for (auto [entity, piece, transform] : piecesViewTransform.each()) {
        glm::vec3 translationDirection;
        if (centerPiece == &piece) {
            translationDirection = glm::vec3(0.0f);
        } else {
            translationDirection = piece.initialPosition - centerPiece->initialPosition;
        }

        transform.position = piece.initialPosition + translationDirection * explodedView.offset;
    }
}

void PuzzleViewSystem::updateSolution() {
    auto puzzleView = scene.registry.view<Puzzle>();
    auto piecesView = scene.registry.view<PuzzlePiece, Transform, Solution>();

    if (puzzleView.empty()) {
        return;
    }

    auto puzzleEntity = puzzleView.front();
    auto& puzzle = scene.registry.get<Puzzle>(puzzleEntity);
    float current_step = puzzle.solutionStep;

    for (auto [entity, piece, transform, solution] : piecesView.each()) 
    {
        if(solution.Solution.size() == 0) {continue;}
        if(current_step > 1.0)
        {
            transform.position = getBezierPoint(solution.Solution, 1.f);
            current_step -= 1;
        }
        else if(current_step < 0.0)
        {
            transform.position = getBezierPoint(solution.Solution, 0.f);
        }
        else
        {
            transform.position = getBezierPoint(solution.Solution, current_step);
            current_step -= 1;
        }
    }
}

void PuzzleViewSystem::updatePuzzleRotation() {
    auto puzzleView = scene.registry.view<Puzzle>();
    if (puzzleView.empty()) {
        return;
    }

    auto puzzleEntity = puzzleView.front();
    auto& puzzle = scene.registry.get<Puzzle>(puzzleEntity);
    if (puzzle.disableMouseRotation) {
        return;
    }

    int state = InputSystem::getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        double xpos, ypos;
        InputSystem::getCursorPos(xpos, ypos);

        if (!mouseHoveringOverPieceView()) {
            auto offset = glm::vec2 (xpos, ypos) - prevMousePos;
            offset.y = -offset.y;

            auto transform = scene.registry.try_get<Transform>(puzzleEntity);
            if (transform != nullptr) {
                transform->rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
            }
        }

        prevMousePos = glm::vec2 (xpos, ypos);
    }
}

void PuzzleViewSystem::updateSelectedPieceColor() {
    pieceSelected = false;

    for (auto [entity, piece] : scene.registry.view<PuzzlePiece>().each()) {
        auto& children = scene.registry.get<Children>(entity).children;
        for (auto block : children) {
            auto& material = scene.registry.get<Material>(block);
            if (piece.selected) {
                pieceSelected = true;
                material.color = piece.selectionColor;
            } else {
                material.color = piece.defaultColor;
            }
        }
    }
}

void PuzzleViewSystem::updatePieceTransparency() {
    for (auto [entity, piece] : scene.registry.view<PuzzlePiece>().each()) {
        auto& children = scene.registry.get<Children>(entity).children;
        for (auto block : children) {
            auto& material = scene.registry.get<Material>(block);
            if (pieceSelected && !piece.selected) {
                material.transparency = piece.otherPieceSelectedTransparency;
            } else {
                material.transparency = 1.0f;
            }
        }
    }
}

void PuzzleViewSystem::updatePieceSelection() {
    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);

    auto& camera = scene.registry.get<Camera>(scene.registry.view<Camera>().front());
    auto view = camera.viewMatrix();

    double x, y;
    InputSystem::getCursorPos(x, y);

    float viewPortWorldHeight = 2 * camera.near * tan(glm::radians(camera.fov / 2));
    float viewPortWorldWidth = aspect * viewPortWorldHeight;

    x = (x / float(WINDOW_WIDTH)) * viewPortWorldWidth - (viewPortWorldWidth / 2);
    y = (y / float(WINDOW_HEIGHT)) * viewPortWorldHeight - (viewPortWorldHeight / 2);

    auto cursorCameraPos = glm::vec4(x, y, -camera.near, 1);
    auto cursorWorldPos = glm::inverse(view) * cursorCameraPos;

    entt::entity closestPiece;
    float lambda = std::numeric_limits<float>::infinity();

    for (auto [entity, piece] : scene.registry.view<PuzzlePiece>().each()) {
        auto& children = scene.registry.get<Children>(entity);
        float intersectionLambda = std::numeric_limits<float>::infinity();

        bool intersectsPiece = false;
        for (auto block : children.children) {
            auto& transform = scene.registry.get<Transform>(block);
            auto& boundingBox = scene.registry.get<BoundingBox>(block);

            auto inverseModel = glm::inverse(getModelMatrix(scene.registry, block));
            auto cameraModelPos = glm::vec3(inverseModel * glm::vec4(camera.position, 1));
            auto cursorModelPos = glm::vec3(inverseModel * cursorWorldPos);

            auto rayDirection = cursorModelPos - cameraModelPos;
            
            float intersectionLambda;
            bool intersects = getRayBoundingBoxIntersection(cameraModelPos, rayDirection, boundingBox, intersectionLambda);

            if (intersects && intersectionLambda < lambda) {
                closestPiece = entity;
                lambda = intersectionLambda;
            }
        }
    }

    for (auto [entity, piece] : scene.registry.view<PuzzlePiece>().each()) {
        if (!std::isinf(lambda) && entity == closestPiece) {
            piece.selected = true;
        } else {
            piece.selected = false;
        }
    }
}

bool PuzzleViewSystem::mouseHoveringOverPieceView() {
    auto pieceViewView = scene.registry.view<PiecesView>();
    if (pieceViewView.empty()) {
        return false;
    }

    double xpos, ypos;
    InputSystem::getCursorPos(xpos, ypos);

    auto pieceView = pieceViewView.front();
    auto& pieceViewTransform = scene.registry.get<Transform2D>(pieceView);
    auto& pieceViewCanvas = scene.registry.get<CanvasElement>(pieceView);

    auto pieceViewLeft = pieceViewCanvas.left + pieceViewTransform.position.x;
    auto pieceViewRight = pieceViewCanvas.right + pieceViewTransform.position.x;
    auto pieceViewTop = pieceViewCanvas.top + pieceViewTransform.position.y;
    auto pieceViewBottom = pieceViewCanvas.bottom + pieceViewTransform.position.y;

    return ((xpos > pieceViewLeft && xpos < pieceViewRight) &&
            (ypos > pieceViewBottom && ypos < pieceViewTop));
}

bool PuzzleViewSystem::getRayBoundingBoxIntersection(glm::vec3 rayStart, glm::vec3 rayDirection, BoundingBox boundingBox, float& intersectionLambda) {
    float lambdaX = std::numeric_limits<float>::infinity();
    if (rayDirection.x != 0) {
        float lambdaX1 = ((-boundingBox.right) - rayStart.x) / rayDirection.x;
        float lambdaX2 = ((boundingBox.right) - rayStart.x) / rayDirection.x;
        if (lambdaX1 >= 0 && lambdaX1 < lambdaX) {
            lambdaX = lambdaX1;
        }
        if (lambdaX2 >= 0 && lambdaX2 < lambdaX) {
            lambdaX = lambdaX2;
        }
        if (!std::isinf(lambdaX)) {
            auto intersect = rayStart + rayDirection * lambdaX;
            if ((intersect.y > -boundingBox.top && intersect.y < boundingBox.top) &&
                (intersect.z > -boundingBox.front && intersect.z < boundingBox.front)) 
            {
                intersectionLambda = lambdaX;
                return true;
            }
        }
    }

    float lambdaY = std::numeric_limits<float>::infinity();
    if (rayDirection.y != 0) {
        float lambdaY1 = ((boundingBox.top) - rayStart.y) / rayDirection.y;
        float lambdaY2 = ((-boundingBox.top) - rayStart.y) / rayDirection.y;
        if (lambdaY1 >= 0 && lambdaY1 < lambdaY) {
            lambdaY = lambdaY1;
        }
        if (lambdaY2 >= 0 && lambdaY2 < lambdaY) {
            lambdaY = lambdaY2;
        }
        if (!std::isinf(lambdaY)) {
            auto intersect = rayStart + rayDirection * lambdaY;
            if ((intersect.z > -boundingBox.front && intersect.z < boundingBox.front) &&
                (intersect.x > -boundingBox.right && intersect.x < boundingBox.right)) 
            {
                intersectionLambda = lambdaY;
                return true;
            }
        }
    }

    float lambdaZ = std::numeric_limits<float>::infinity();
    if (rayDirection.z != 0) {
        float lambdaZ1 = ((boundingBox.front) - rayStart.z) / rayDirection.z;
        float lambdaZ2 = ((-boundingBox.front) - rayStart.z) / rayDirection.z;
        if (lambdaZ1 >= 0 && lambdaZ1 < lambdaZ) {
            lambdaZ = lambdaZ1;
        }
        if (lambdaZ2 >= 0 && lambdaZ2 < lambdaZ) {
            lambdaZ = lambdaZ2;
        }
        if (!std::isinf(lambdaZ)) {
            auto intersect = rayStart + rayDirection * lambdaZ;
            if ((intersect.y > -boundingBox.top && intersect.y < boundingBox.top) &&
                (intersect.x > -boundingBox.right && intersect.x < boundingBox.right)) 
            {
                intersectionLambda = lambdaZ;
                return true;
            }
        }
    }

    return false;
}

glm::vec3 PuzzleViewSystem::getBezierPoint( std::vector<glm::vec3> points, float t ) {
    std::vector<glm::vec3> tmp(points);
    
    for(int i = points.size() - 1; i > 0; i--)
    {
        for (int k = 0; k < i; k++)
            tmp[k] = tmp[k] + t * ( tmp[k+1] - tmp[k] );
        i--;
    }

    return tmp[0];
}

void PuzzleViewSystem::scrollCallback(InputSystem::ScrollEvent scrollEvent) {
    if (mouseHoveringOverPieceView()) {
        return;
    }

    auto cameraView = scene.registry.view<Camera>();
    if (cameraView.empty()) {
        return;
    }

    auto& camera = scene.registry.get<Camera>(cameraView.back());
    camera.fov = glm::clamp(camera.fov - float(scrollEvent.yoffset) * ZOOM_SPEED, 5.0f, 175.0f);
}

void PuzzleViewSystem::mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent) {
    if (mouseButtonCallbackEvent.button == GLFW_MOUSE_BUTTON_LEFT && mouseButtonCallbackEvent.action == GLFW_PRESS) {
        double xpos, ypos;
        InputSystem::getCursorPos(xpos, ypos);
        prevMousePos = glm::vec2(xpos, ypos);
    }
    else if (mouseButtonCallbackEvent.button == GLFW_MOUSE_BUTTON_RIGHT && mouseButtonCallbackEvent.action == GLFW_PRESS) {
        if (mouseHoveringOverPieceView()) {
            return;
        }
        updatePieceSelection();
    }
}