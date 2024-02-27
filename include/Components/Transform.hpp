#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/glm.hpp"

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif //TRANSFORM_HPP