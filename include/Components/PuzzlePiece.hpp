#ifndef PUZZLE_PIECE_HPP
#define PUZZLE_PIECE_HPP

#include "glm/glm.hpp"

struct PuzzlePiece {
    glm::vec3 initialPosition;
    glm::vec3 initialRotation;

    std::vector<glm::vec3> CubePos;

    bool selected;
    glm::vec3 defaultColor;
    glm::vec3 selectionColor;
    float otherPieceSelectedTransparency;
};

#endif //PUZZLE_PIECE_HPP
