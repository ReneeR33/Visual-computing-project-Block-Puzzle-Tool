#include "Systems/PuzzleViewSystem.hpp"

#include "Components/ExplodedView.hpp"
#include "Components/Transform.hpp"
#include "Components/PuzzlePiece.hpp"

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
    //auto windowHandle = puzzleViewSystem->window.getHandle();
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
    std::cout << xoffset << ", " << yoffset << std::endl;
}
