#ifndef PIECE_VIEW_SYSTEM
#define PIECE_VIEW_SYSTEM

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "InputSystem.hpp"

class PieceViewSystem {
public:
    PieceViewSystem(entt::registry& scene);
    void update();

private:
    entt::registry& scene;

    glm::vec2 prevMousePos;

    //entt::entity selectedPiece;
    //entt::entity selectedSinglePieceView;

    void updatePieceRotation(entt::entity pieceSubsceneEntity, double mouseXPos, double mouseYPos);

    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PIECE_VIEW_SYSTEM