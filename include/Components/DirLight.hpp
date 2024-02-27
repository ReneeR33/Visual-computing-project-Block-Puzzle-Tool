#ifndef DIR_LIGHT_HPP
#define DIR_LIGHT_HPP

#include "glm/glm.hpp"

struct DirLight {
    glm::vec3 direction;
    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
};

#endif //DIR_LIGHT_HPP