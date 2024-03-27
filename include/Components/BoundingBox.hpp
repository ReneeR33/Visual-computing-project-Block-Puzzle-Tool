#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include "glm/glm.hpp"

struct BoundingBox {
    glm::vec3 center;
    glm::vec3 right;
    glm::vec3 top;
    glm::vec3 front;
};

#endif //BOUNDING_BOX_HPP