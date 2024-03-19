#include "Systems/PieceViewSystem.hpp"

#include "Components/PiecesView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/ScrollView.hpp"
#include "Components/SinglePieceView.hpp"
#include "Components/UIScene.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Transform.hpp"

#include <iostream>

#include "UI.hpp"

#define SCROLL_SPEED 70.0f
#define ROTATE_SPEED 0.25f

PieceViewSystem::PieceViewSystem(entt::registry& scene) 
    : scene(scene) {
    InputSystem::event<InputSystem::ScrollEvent>().connect<&PieceViewSystem::scrollCallback>(*this);
    InputSystem::event<InputSystem::MouseButtonCallBackEvent>().connect<&PieceViewSystem::mouseButtonCallback>(*this);
}

void PieceViewSystem::update() {
    double xpos, ypos;
    InputSystem::getCursorPos(xpos, ypos);

    for (auto [entity, scrollview] : scene.view<ScrollView>().each()) {
        auto children = getScrollViewChildren(scene, entity);
        for (auto singlePieceView : children) {
            auto& canvas = scene.get<CanvasElement>(singlePieceView);
            auto screenPosition = UIEntityScreenPosition(scene, singlePieceView);

            if ((xpos > (screenPosition.x + canvas.left) && xpos < (screenPosition.x + canvas.right)) &&
                (ypos > (screenPosition.y + canvas.bottom) && ypos < (screenPosition.y + canvas.top))) 
            {
                int state = InputSystem::getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
                if (state == GLFW_PRESS) {
                    auto offset = glm::vec2 (xpos, ypos) - prevMousePos;
                    offset.y = -offset.y;

                    auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);
                    auto& subsceneComponent = scene.get<UIScene>(singlePieceViewComponent.subscene);

                    for (auto [entity, piece, transform] : subsceneComponent.scene.view<PuzzlePiece, Transform>().each()) {
                        transform.rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
                    }

                    prevMousePos = glm::vec2 (xpos, ypos);
                }
            }
        }
    }
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

void PieceViewSystem::mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent) {
    if (mouseButtonCallbackEvent.button == GLFW_MOUSE_BUTTON_LEFT && mouseButtonCallbackEvent.action == GLFW_PRESS) {
        double xpos, ypos;
        InputSystem::getCursorPos(xpos, ypos);
        prevMousePos = glm::vec2(xpos, ypos);
    }
}