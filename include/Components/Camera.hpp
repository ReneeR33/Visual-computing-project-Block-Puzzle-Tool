#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

struct Camera {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    float near;
    float far;
    float fov;

    glm::mat4 viewMatrix();
};

#endif //CAMERA_HPP