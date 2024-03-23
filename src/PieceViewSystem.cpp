#include "Systems/PieceViewSystem.hpp"

#include "Components/PiecesView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/ScrollView.hpp"
#include "Components/SinglePieceView.hpp"
#include "Components/UIScene.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Transform.hpp"
#include "Components/Fill2D.hpp"
#include "Components/Puzzle.hpp"
#include "Components/Children.hpp"

#include <iostream>

#include "UI.hpp"

#define SCROLL_SPEED 70.0f
#define ROTATE_SPEED 0.4f

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

            auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);
            auto& backGroundFill = scene.get<Fill2D>(singlePieceViewComponent.background);

            auto& piece = scene.get<PuzzlePiece>(singlePieceViewComponent.piece);

            if ((xpos > (screenPosition.x + canvas.left) && xpos < (screenPosition.x + canvas.right)) &&
                (ypos > (screenPosition.y + canvas.bottom) && ypos < (screenPosition.y + canvas.top))) 
            {
                int state = InputSystem::getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
                if (state == GLFW_PRESS) {
                    updatePieceRotation(singlePieceViewComponent.subscene, xpos, ypos);
                }

                if (!piece.selected) {
                    backGroundFill.color = singlePieceViewComponent.hoverBackgroundColor;
                }
            } else {
                if (!piece.selected) {
                    backGroundFill.color = singlePieceViewComponent.backgroundColor;  
                }
            }

            if (piece.selected) {
                backGroundFill.color = singlePieceViewComponent.hoverBackgroundColor + glm::vec3(0.2f);

                if (singlePieceViewComponent.piece != selectedPiece) {
                    auto& singlePieceTransform = scene.get<Transform2D>(singlePieceView);
                    // this doesn't move it precisely int the middle I think but should be good enough for now...
                    // TODO: make sure the piece is now positioned in the middle of the piece view scroll box!
                    scrollview.value = -singlePieceTransform.position.y - canvas.top - 150.0f;
                    selectedPiece = singlePieceViewComponent.piece;
                }
            }
        }
    }
}

void PieceViewSystem::updatePieceRotation(entt::entity pieceSubsceneEntity, double mouseXPos, double mouseYPos) {
    auto offset = glm::vec2 (mouseXPos, mouseYPos) - prevMousePos;
    offset.y = -offset.y;

    auto& subsceneComponent = scene.get<UIScene>(pieceSubsceneEntity);

    for (auto [entity, piece, transform] : subsceneComponent.scene.view<PuzzlePiece, Transform>().each()) {
        transform.rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
    }

    prevMousePos = glm::vec2 (mouseXPos, mouseYPos);
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

        for (auto [entity, pieceView] : scene.view<PiecesView>().each()) {
            auto scrollView = pieceView.scrollView;
            auto children = getScrollViewChildren(scene, scrollView);

            for (auto singlePieceView : children) {
                auto& canvas = scene.get<CanvasElement>(singlePieceView);
                auto screenPosition = UIEntityScreenPosition(scene, singlePieceView);

                if ((xpos > (screenPosition.x + canvas.left) && xpos < (screenPosition.x + canvas.right)) &&
                    (ypos > (screenPosition.y + canvas.bottom) && ypos < (screenPosition.y + canvas.top))) 
                {
                    auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);

                    auto& puzzleChildren = scene.get<Children>(pieceView.puzzle);
                    for (auto puzzlePiece : puzzleChildren.children) {
                        auto& pieceComponent = scene.get<PuzzlePiece>(puzzlePiece);
                        if (puzzlePiece == singlePieceViewComponent.piece) {
                            pieceComponent.selected = true;
                            selectedPiece = puzzlePiece;
                        } else {
                            pieceComponent.selected = false;
                        }
                    }
                }
            }
        }
    }
}