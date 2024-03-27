#include "Components/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

glm::mat4 Transform::modelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, scale);

    return model;
}

glm::mat4 Transform::inverseModelMatrix() {
    glm::mat4 model(1.0f);

    model = glm::scale(model, glm::vec3(1 / scale.x, 1 / scale.y, 1 / scale.z));
    model = glm::rotate(model, glm::radians(-rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(-rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(-rotation.z), glm::vec3(0, 0, 1));
    model = glm::translate(model, -position);

    return model;
}

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