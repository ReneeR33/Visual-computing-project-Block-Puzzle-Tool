#ifndef TRANSFORM_2D
#define TRANSFORM_2D

#include "glm/glm.hpp"

struct Transform2D {
    glm::vec2 position;
    float rotation;
    glm::vec2 scale;
};

#endif //TRANSFORM_2D
