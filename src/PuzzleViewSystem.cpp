#include "Systems/PuzzleViewSystem.hpp"

#include "Components/ExplodedView.hpp"
#include "Components/Transform.hpp"

void PuzzleViewSystem::Update(entt::registry& scene) {
    auto explodedViewView = scene.view<ExplodedView>();
    if (explodedViewView.empty()) {
        return;
    }
    auto& exploded_view = scene.get<ExplodedView>(explodedViewView.front());

    //TODO: filter based on puzzle piece component
    auto piecesView = scene.view<Transform>();
    if (piecesView.empty()) {
        return;
    }

    Transform* centerPiece = nullptr;
    for (auto [entity, transform] : piecesView.each()) {
        if (centerPiece == nullptr || glm::length(transform.position) < glm::length(centerPiece->position)) {
            centerPiece = &transform;
        }
    }


}