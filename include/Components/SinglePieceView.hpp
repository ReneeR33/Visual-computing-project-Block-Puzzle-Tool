#ifndef SINGLE_PIECE_VIEW_HPP
#define SINGLE_PIECE_VIEW_HPP

#include <entt/entt.hpp>
#include "glm/glm.hpp"

struct SinglePieceView {
    glm::vec3 backgroundColor;
    glm::vec3 hoverBackgroundColor;

    entt::entity subscene;
    entt::entity background;
};

#endif //SINGLE_PIECE_VIEW_HPP