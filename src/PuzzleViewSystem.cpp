#include "Systems/PuzzleViewSystem.hpp"

#include "Components/ExplodedView.hpp"
#include "Components/Transform.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Camera.hpp"

#include <iostream>

PuzzleViewSystem* PuzzleViewSystem::puzzleViewSystem = nullptr;

PuzzleViewSystem::PuzzleViewSystem(entt::registry &scene, GlfwWindow &window)
    : scene(scene)
    , window(window)
{
    glfwSetScrollCallback(window.getHandle(), scrollCallback);
}

void PuzzleViewSystem::init(entt::registry &scene, GlfwWindow &window) {
    delete puzzleViewSystem;
    puzzleViewSystem = new PuzzleViewSystem(scene, window);
}

void PuzzleViewSystem::update() {
    if (puzzleViewSystem == nullptr) {
        return;
    }
    updateExplodedView();
}

void PuzzleViewSystem::updateExplodedView() {
    auto& scene = puzzleViewSystem->scene;

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
            translationDirection = glm::normalize(piece.initialPosition - centerPiece->initialPosition);
        }

        transform.position = piece.initialPosition + translationDirection * explodedView.offset;
    }
}

void PuzzleViewSystem::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    auto& scene = puzzleViewSystem->scene;

    auto cameraView = scene.view<Camera>();
    if (cameraView.empty()) {
        return;
    }

    auto& camera = scene.get<Camera>(cameraView.back());
    camera.fov = glm::clamp(camera.fov + float(yoffset) * 5.0f, 5.0f, 175.0f);
}