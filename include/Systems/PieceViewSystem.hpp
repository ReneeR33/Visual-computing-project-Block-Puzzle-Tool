#ifndef PIECE_VIEW_SYSTEM
#define PIECE_VIEW_SYSTEM

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "InputSystem.hpp"
#include "Scene.hpp"

class PieceViewSystem {
public:
    PieceViewSystem(Scene& scene);
    void update();

private:
    Scene& scene;

    glm::vec2 prevMousePos;

    entt::entity selectedPiece;

    void updatePieceRotation(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView, double mouseXPos, double mouseYPos);
    void updateScrollViewScrollValue(double yoffset);
    void updateSinglePieceViewBackgroundColor(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView);
    void updateScrollViewScrollValueWhenSelectedPieceChanged(entt::entity pieceView, entt::entity singlePieceView);

    void updateSelectedPiece(entt::entity pieceView, entt::entity singlePieceView, bool mouseOnSinglePieceView);

    bool pointOnUIEntity(entt::entity uiEntity, double xpos, double ypos);

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PIECE_VIEW_SYSTEM