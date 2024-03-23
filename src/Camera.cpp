#include "Components/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>


glm::mat4 Camera::viewMatrix() {
    glm::mat4 view = glm::lookAt(position, position + direction, glm::normalize(up));
    return view;
}