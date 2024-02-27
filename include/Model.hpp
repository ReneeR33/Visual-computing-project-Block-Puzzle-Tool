#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include "Mesh.hpp"

struct Model {
    std::vector<Mesh> meshes;
    glm::vec3 color;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    float specularPow;
};

#endif //MODEL_HPP