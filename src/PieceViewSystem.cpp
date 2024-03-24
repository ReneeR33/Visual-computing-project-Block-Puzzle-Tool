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

    for (auto [entity, pieceView] : scene.view<PiecesView>().each()) {
        auto children = getScrollViewChildren(scene, pieceView.scrollView);
        for (auto singlePieceView : children) {
            auto& canvas = scene.get<CanvasElement>(singlePieceView);
            auto screenPosition = UIEntityScreenPosition(scene, singlePieceView);

            auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);
            auto& backGroundFill = scene.get<Fill2D>(singlePieceViewComponent.background);

            auto& piece = scene.get<PuzzlePiece>(singlePieceViewComponent.piece);

            bool mouseOnSinglePieceView = pointOnUIEntity(singlePieceView, xpos, ypos);
            updatePieceRotation(entity, singlePieceView, mouseOnSinglePieceView, xpos, ypos);
            updateSinglePieceViewBackgroundColor(entity, singlePieceView, mouseOnSinglePieceView);
            updateScrollViewScrollValueWhenSelectedPieceChanged(entity, singlePieceView);
        }
    }
}

void PieceViewSystem::updatePieceRotation(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView, double mouseXPos, double mouseYPos) {
    if (mouseOnSinglePieceView) {
        auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);

        int state = InputSystem::getMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS) {
            auto offset = glm::vec2 (mouseXPos, mouseYPos) - prevMousePos;
            offset.y = -offset.y;

            auto& subsceneComponent = scene.get<UIScene>(singlePieceViewComponent.subscene);

            for (auto [entity, piece, transform] : subsceneComponent.scene.view<PuzzlePiece, Transform>().each()) {
                transform.rotate(offset.y * ROTATE_SPEED, offset.x * ROTATE_SPEED,0.0f);
            }

            prevMousePos = glm::vec2 (mouseXPos, mouseYPos);
        }
    }
}

void PieceViewSystem::updateScrollViewScrollValue(double yoffset) {
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
        pieceViewScrollComponent.value += -float(yoffset) * SCROLL_SPEED;
    }
}

void PieceViewSystem::updateSinglePieceViewBackgroundColor(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView) {
    auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);
    auto& piece = scene.get<PuzzlePiece>(singlePieceViewComponent.piece);
    auto& singlePieceViewBackGroundFill = scene.get<Fill2D>(singlePieceViewComponent.background);

    if (mouseOnSinglePieceView) 
    {
        if (!piece.selected) {
            singlePieceViewBackGroundFill.color = singlePieceViewComponent.hoverBackgroundColor;
        }
    } else {
        if (!piece.selected) {
            singlePieceViewBackGroundFill.color = singlePieceViewComponent.backgroundColor;  
        }
    }

    if (piece.selected) {
        singlePieceViewBackGroundFill.color = singlePieceViewComponent.hoverBackgroundColor + glm::vec3(0.2f);
    }
}

void PieceViewSystem::updateScrollViewScrollValueWhenSelectedPieceChanged(entt::entity pieceView, entt::entity singlePieceView) {
    auto& pieceViewComponent = scene.get<PiecesView>(pieceView);
    auto& pieceViewScrollView = scene.get<ScrollView>(pieceViewComponent.scrollView);

    auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);
    auto& singlePieceViewCanvas = scene.get<CanvasElement>(singlePieceView);
    auto& piece = scene.get<PuzzlePiece>(singlePieceViewComponent.piece);

    if (piece.selected && singlePieceViewComponent.piece != selectedPiece) {
        auto& singlePieceTransform = scene.get<Transform2D>(singlePieceView);
        // this doesn't move it precisely in the middle I think but should be good enough for now...
        // TODO: make sure the piece is now positioned in the middle of the piece view scroll box!
        pieceViewScrollView.value = -singlePieceTransform.position.y - singlePieceViewCanvas.top - 150.0f;

        // TODO: move this somewhere else?
        selectedPiece = singlePieceViewComponent.piece;
    }
}

void PieceViewSystem::updateSelectedPiece(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView) {
    if (mouseOnSinglePieceView) {
        auto& pieceViewComponent = scene.get<PiecesView>(pieceView);
        auto& singlePieceViewComponent = scene.get<SinglePieceView>(singlePieceView);

        auto& puzzleChildren = scene.get<Children>(pieceViewComponent.puzzle);
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

// TODO: move this to the UI header?
bool PieceViewSystem::pointOnUIEntity(entt::entity uiEntity, double xpos, double ypos) {
    auto& canvas = scene.get<CanvasElement>(uiEntity);
    auto screenPosition = UIEntityScreenPosition(scene, uiEntity);

    if ((xpos > (screenPosition.x + canvas.left) && xpos < (screenPosition.x + canvas.right)) &&
        (ypos > (screenPosition.y + canvas.bottom) && ypos < (screenPosition.y + canvas.top)))
    {
        return true;
    }

    return false;
}

void PieceViewSystem::scrollCallback(InputSystem::ScrollEvent scrollEvent) {
    updateScrollViewScrollValue(scrollEvent.yoffset);
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
                bool mouseHoveringOverPieceView = pointOnUIEntity(singlePieceView, xpos, ypos);

                updateSelectedPiece(entity, singlePieceView, mouseHoveringOverPieceView);
            }
        }
    }
}