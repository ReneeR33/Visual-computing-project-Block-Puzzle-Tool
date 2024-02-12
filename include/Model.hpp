#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include "Mesh.hpp"

struct Model {
    std::vector<Mesh> meshes;
};

#endif //MODEL_HPP