#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "glm/glm.hpp"


struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 modelMatrix();
    glm::mat4 inverseModelMatrix();

    void rotate(float x, float y, float z);
};

#endif //TRANSFORM_HPP