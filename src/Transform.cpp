#include "Components/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

void Transform::rotate(float x, float y, float z) {
    auto transformRotationMatrix = glm::eulerAngleXYZ(
            glm::radians(rotation.x),
            glm::radians(rotation.y),
            glm::radians(rotation.z));
    auto rotationMatrix = glm::eulerAngleXYZ(
            glm::radians(x),
            glm::radians(y),
            glm::radians(z));

    transformRotationMatrix = rotationMatrix * transformRotationMatrix;

    glm::vec3 rotationRadians;
    glm::extractEulerAngleXYZ(transformRotationMatrix, rotationRadians.x, rotationRadians.y, rotationRadians.z);

    rotation.x = glm::degrees(rotationRadians.x);
    rotation.y = glm::degrees(rotationRadians.y);
    rotation.z = glm::degrees(rotationRadians.z);
}