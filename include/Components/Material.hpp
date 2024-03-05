#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "glm/glm.hpp"

struct Material {
    glm::vec4 color;
    glm::vec3 ambientColor;
    glm::vec3 specularColor;
    float specularPow;
};

#endif //MATERIAL_HPP