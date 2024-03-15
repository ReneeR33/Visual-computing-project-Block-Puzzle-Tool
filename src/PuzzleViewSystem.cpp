#include "Systems/PuzzleViewSystem.hpp"

#include "Components/ExplodedView.hpp"
#include "Components/Transform.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Camera.hpp"
#include "Components/Puzzle.hpp"

#define ZOOM_SPEED 5.0f
#define ROTATE_SPEED 0.25f

// PuzzleViewSystem* PuzzleViewSystem::puzzleViewSystem = nullptr;

/*PuzzleViewSystem::PuzzleViewSystem(entt::registry &scene, GlfwWindow &window)
    : scene(scene)
    , window(window)
{
    glfwSetScrollCallback(window.getHandle(), scrollCallback);
    glfwSetMouseButtonCallback(window.getHandle(), mouseButtonCallback);
}*/

PuzzleViewSystem::PuzzleViewSystem(entt::registry &scene)
    : scene(scene) {
    InputSystem::event<InputSystem::ScrollEvent>().connect<&PuzzleViewSystem::scrollCallback>(*this);
    InputSystem::event<InputSystem::MouseButtonCallBackEvent>().connect<&PuzzleViewSystem::mouseButtonCallback>(*this);
}

/*void PuzzleViewSystem::init(entt::registry &scene, GlfwWindow &window) {
    delete puzzleViewSystem;
    puzzleViewSystem = new PuzzleViewSystem(scene, window);
}*/

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
        auto offset = glm::vec2 (xpos, ypos) - prevMousePos;
        offset.y = -offset.y;

        auto transform = scene.try_get<Transform>(puzzleEntity);
        if (transform != nullptr) {
            transform->rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
        }

        prevMousePos = glm::vec2 (xpos, ypos);
    }
}

void PuzzleViewSystem::scrollCallback(InputSystem::ScrollEvent scrollEvent) {
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
    }
}

/*void PuzzleViewSystem::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    auto& scene = puzzleViewSystem->scene;

    auto cameraView = scene.view<Camera>();
    if (cameraView.empty()) {
        return;
    }

    auto& camera = scene.get<Camera>(cameraView.back());
    camera.fov = glm::clamp(camera.fov - float(yoffset) * ZOOM_SPEED, 5.0f, 175.0f);
}

void PuzzleViewSystem::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto& prevMousePos = puzzleViewSystem->prevMousePos;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        prevMousePos = glm::vec2 (xpos, ypos);
    }
}*/
