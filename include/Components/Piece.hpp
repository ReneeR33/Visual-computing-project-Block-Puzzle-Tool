#ifndef PIECE_HPP
#define PIECE_HPP

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Piece {
    glm::vec3 SolutionPosition;
    std::vector<glm::vec3> CubePos;
    bool selected;
};

#endif //PIECE_HPP