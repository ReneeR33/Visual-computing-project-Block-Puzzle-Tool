#include "Systems/PieceViewSystem.hpp"

#include "Components/PiecesView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/ScrollView.hpp"

#define SCROLL_SPEED 50.0f

PieceViewSystem::PieceViewSystem(entt::registry& scene) 
    : scene(scene) {
    //InputSystem::event<InputSystem::ScrollEvent>().connect<&PieceViewSystem::scrollCallback>(*this);
    //auto& thing = *this;
    InputSystem::event<InputSystem::ScrollEvent>().connect<&PieceViewSystem::scrollCallback>(*this);
}

/*void PieceViewSystem::init(entt::registry &scene, GlfwWindow &window) {
    if (pieceViewSystem == nullptr) {
        glfwSetScrollCallback(window.getHandle(), scrollCallback);
    }

    delete pieceViewSystem;
    pieceViewSystem = new PieceViewSystem(scene, window);
}*/

void PieceViewSystem::update() {

}

void PieceViewSystem::scrollCallback(InputSystem::ScrollEvent scrollEvent) {
    auto pieceViewView = scene.view<PiecesView>();
    if (pieceViewView.empty()) {
        return;
    }

    auto pieceView = pieceViewView.front();
    auto& pieceViewTransform = scene.get<Transform2D>(pieceView);
    auto& pieceViewCanvas = scene.get<CanvasElement>(pieceView);

    double xpos, ypos;
    InputSystem::getCursorPos(xpos, ypos);

    auto pieceViewLeft = pieceViewCanvas.left + pieceViewTransform.position.x;
    auto pieceViewRight = pieceViewCanvas.right + pieceViewTransform.position.x;
    auto pieceViewTop = pieceViewCanvas.top + pieceViewTransform.position.y;
    auto pieceViewBottom = pieceViewCanvas.bottom + pieceViewTransform.position.y;

    if ((xpos > pieceViewLeft && xpos < pieceViewRight) &&
        (ypos > pieceViewBottom && ypos < pieceViewTop)) {
        auto& pieceViewComponent = scene.get<PiecesView>(pieceView);
        auto& pieceViewScrollComponent = scene.get<ScrollView>(pieceViewComponent.scrollView);
        pieceViewScrollComponent.value += -float(scrollEvent.yoffset) * SCROLL_SPEED;
    }
}

/*void PieceViewSystem::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if (pieceViewSystem == nullptr) {
        return;
    }

    auto& scene = pieceViewSystem->scene;
    auto& glfwWindow = pieceViewSystem->window;

    auto pieceViewView = scene.view<PiecesView>();
    if (pieceViewView.empty()) {
        return;
    }

    auto pieceView = pieceViewView.front();
    auto& pieceViewTransform = scene.get<Transform2D>(pieceView);
    auto& pieceViewCanvas = scene.get<CanvasElement>(pieceView);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    ypos = glfwWindow.getHeight() - ypos;

    auto pieceViewLeft = pieceViewCanvas.left + pieceViewTransform.position.x;
    auto pieceViewRight = pieceViewCanvas.right + pieceViewTransform.position.x;
    auto pieceViewTop = pieceViewCanvas.top + pieceViewTransform.position.y;
    auto pieceViewBottom = pieceViewCanvas.bottom + pieceViewTransform.position.y;

    if ((xpos > pieceViewLeft && xpos < pieceViewRight) &&
        (ypos > pieceViewBottom && ypos < pieceViewTop)) {
        auto& pieceViewComponent = scene.get<PiecesView>(pieceView);
        auto& pieceViewScrollComponent = scene.get<ScrollView>(pieceViewComponent.scrollView);
        pieceViewScrollComponent.value += -float(yoffset) * SCROLL_SPEED;
    }
}*/