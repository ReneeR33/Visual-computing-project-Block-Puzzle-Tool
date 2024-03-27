#ifndef MODEL_DATA_HPP
#define MODEL_DATA_HPP

#include <vector>
#include "Mesh.hpp"

struct ModelData {
    std::vector<Mesh> meshes;
};

#endif //MODEL_DATA_HPP