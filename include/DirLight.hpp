#ifndef DIR_LIGHT_HPP
#define DIR_LIGHT_HPP

#include <glm/glm.hpp>

struct DirLight {
    glm::vec3 direction;
    glm::vec3 diffuse;
};

#endif //DIR_LIGHT_HPP