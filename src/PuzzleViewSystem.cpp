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

#include <iostream>

//TODO: move this somewhere else
#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 950

#define ZOOM_SPEED 5.0f
#define ROTATE_SPEED 0.25f

PuzzleViewSystem::PuzzleViewSystem(entt::registry &scene)
    : scene(scene) {
    InputSystem::event<InputSystem::ScrollEvent>().connect<&PuzzleViewSystem::scrollCallback>(*this);
    InputSystem::event<InputSystem::MouseButtonCallBackEvent>().connect<&PuzzleViewSystem::mouseButtonCallback>(*this);
}

void PuzzleViewSystem::update() {
    updateExplodedView();
    updatePuzzleRotation();
}

void PuzzleViewSystem::updateExplodedView() {
    auto explodedViewView = scene.view<ExplodedView>();
    if (explodedViewView.empty()) {
        return;
    }
    auto& explodedView = scene.get<ExplodedView>(explodedViewView.front());

    auto piecesView = scene.view<PuzzlePiece, Transform>();
    PuzzlePiece* centerPiece = nullptr;
    for (auto [entity, piece, transform] : piecesView.each()) {
        if (centerPiece == nullptr || glm::length(piece.initialPosition) < glm::length(centerPiece->initialPosition)) {
            centerPiece = &piece;
        }
    }

    if (centerPiece == nullptr) {
        return;
    }

    auto piecesViewTransform = scene.view<PuzzlePiece, Transform>();
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

void PuzzleViewSystem::updatePuzzleRotation() {
    auto puzzleView = scene.view<Puzzle>();
    if (puzzleView.empty()) {
        return;
    }

    auto puzzleEntity = puzzleView.front();
    auto& puzzle = scene.get<Puzzle>(puzzleEntity);
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

            auto transform = scene.try_get<Transform>(puzzleEntity);
            if (transform != nullptr) {
                transform->rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
            }
        }

        prevMousePos = glm::vec2 (xpos, ypos);
    }
}

void PuzzleViewSystem::updatePieceSelection() {
    float aspectInv = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);

    auto& camera = scene.get<Camera>(scene.view<Camera>().front());
    auto view = camera.viewMatrix();

    double x, y;
    InputSystem::getCursorPos(x, y);

    float viewPortWorldHeight = 2 * tan(glm::radians(camera.fov / 2));
    float viewPortWorldWidth = aspectInv * viewPortWorldHeight;

    x = (x / float(WINDOW_WIDTH)) * viewPortWorldWidth - (viewPortWorldWidth / 2);
    y = (y / float(WINDOW_HEIGHT)) * viewPortWorldHeight - (viewPortWorldHeight / 2);

    auto cursorCameraPos = glm::vec4(x, y, -1, 1);
    auto cursorWorldPos = glm::inverse(view) * cursorCameraPos;

    entt::entity closestPiece;
    float lambda = std::numeric_limits<float>::infinity();

    for (auto [entity, piece] : scene.view<PuzzlePiece>().each()) {
        auto& children = scene.get<Children>(entity);
        float intersectionLambda = std::numeric_limits<float>::infinity();

        bool intersectsPiece = false;
        for (auto block : children.children) {
            auto& transform = scene.get<Transform>(block);
            auto& boundingBox = scene.get<BoundingBox>(block);

            auto inverseModel = glm::inverse(getModelMatrix(scene, block));
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

    if (!isinf(lambda)) {
        std::cout << "intersection!\n";
    }

    //std::cout << cursorWorldPos.x << ", " << cursorWorldPos.y << ", " << cursorWorldPos.z << std::endl;
}

bool PuzzleViewSystem::mouseHoveringOverPieceView() {
    auto pieceViewView = scene.view<PiecesView>();
    if (pieceViewView.empty()) {
        return false;
    }

    double xpos, ypos;
    InputSystem::getCursorPos(xpos, ypos);

    auto pieceView = pieceViewView.front();
    auto& pieceViewTransform = scene.get<Transform2D>(pieceView);
    auto& pieceViewCanvas = scene.get<CanvasElement>(pieceView);

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
        float lambdaX1 = ((boundingBox.center.x - boundingBox.right.x) - rayStart.x) / rayDirection.x;
        float lambdaX2 = ((boundingBox.center.x + boundingBox.right.x) - rayStart.x) / rayDirection.x;
        if (lambdaX1 >= 0 && lambdaX1 < lambdaX) {
            lambdaX = lambdaX1;
        }
        if (lambdaX2 >= 0 && lambdaX2 < lambdaX) {
            lambdaX = lambdaX2;
        }
        if (!isinf(lambdaX)) {
            auto intersect = rayStart + rayDirection * lambdaX;
            if ((intersect.y > boundingBox.center.y - boundingBox.top.y && intersect.y < boundingBox.center.y + boundingBox.top.y) &&
                (intersect.z > boundingBox.center.z - boundingBox.front.z && intersect.z < boundingBox.center.z + boundingBox.front.z)) 
            {
                intersectionLambda = lambdaX;
                return true;
            }
        }
    }

    float lambdaY = std::numeric_limits<float>::infinity();
    if (rayDirection.y != 0) {
        float lambdaY1 = ((boundingBox.center.y + boundingBox.top.y) - rayStart.y) / rayDirection.y;
        float lambdaY2 = ((boundingBox.center.y - boundingBox.top.y) - rayStart.y) / rayDirection.y;
        if (lambdaY1 >= 0 && lambdaY1 < lambdaY) {
            lambdaY = lambdaY1;
        }
        if (lambdaY2 >= 0 && lambdaY2 < lambdaY) {
            lambdaY = lambdaY1;
        }
        if (!isinf(lambdaY)) {
            auto intersect = rayStart + rayDirection * lambdaY;
            if ((intersect.z > boundingBox.center.z - boundingBox.front.z && intersect.z < boundingBox.center.z + boundingBox.front.z) &&
                (intersect.x > boundingBox.center.x - boundingBox.right.x && intersect.x < boundingBox.center.x + boundingBox.right.x)) 
            {
                intersectionLambda = lambdaY;
                return true;
            }
        }
    }

    float lambdaZ = std::numeric_limits<float>::infinity();
    if (rayDirection.z != 0) {
        float lambdaZ1 = ((boundingBox.center.z + boundingBox.front.z) - rayStart.z) / rayDirection.z;
        float lambdaZ2 = ((boundingBox.center.z - boundingBox.front.z) - rayStart.z) / rayDirection.z;
        if (lambdaZ1 >= 0 && lambdaZ1 < lambdaZ) {
            lambdaZ = lambdaZ1;
        }
        if (lambdaZ2 >= 0 && lambdaZ2 < lambdaZ) {
            lambdaZ = lambdaZ2;
        }
        if (!isinf(lambdaZ)) {
            auto intersect = rayStart + rayDirection * lambdaZ;
            if ((intersect.y > boundingBox.center.y - boundingBox.top.y && intersect.y < boundingBox.center.y + boundingBox.top.y) &&
                (intersect.x > boundingBox.center.x - boundingBox.right.x && intersect.x < boundingBox.center.x + boundingBox.right.x)) 
            {
                intersectionLambda = lambdaZ;
                return true;
            }
        }
    }

    return false;
}

void PuzzleViewSystem::scrollCallback(InputSystem::ScrollEvent scrollEvent) {
    if (mouseHoveringOverPieceView()) {
        return;
    }

    auto cameraView = scene.view<Camera>();
    if (cameraView.empty()) {
        return;
    }

    auto& camera = scene.get<Camera>(cameraView.back());
    camera.fov = glm::clamp(camera.fov - float(scrollEvent.yoffset) * ZOOM_SPEED, 5.0f, 175.0f);
}

void PuzzleViewSystem::mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent) {
    if (mouseButtonCallbackEvent.button == GLFW_MOUSE_BUTTON_LEFT && mouseButtonCallbackEvent.action == GLFW_PRESS) {
        double xpos, ypos;
        InputSystem::getCursorPos(xpos, ypos);
        prevMousePos = glm::vec2(xpos, ypos);

        updatePieceSelection();
    }
}