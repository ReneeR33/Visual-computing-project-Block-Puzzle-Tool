#include "entity.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "Components/Parent.hpp"
#include "Components/Transform.hpp"

glm::mat4 getModelMatrix(entt::registry& scene, entt::entity entity) {
    auto cEntity = &entity;
    auto modelm = glm::mat4(1.0f);
    while (cEntity != nullptr) {
        auto transform = scene.try_get<Transform>(*cEntity);

        if (transform != nullptr) {
            auto localmodelm = glm::mat4(1.0f);
            localmodelm = glm::translate(localmodelm, transform->position);
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
            localmodelm = glm::scale(localmodelm, transform->scale);

            modelm = localmodelm * modelm;
        }

        auto parentComponent = scene.try_get<Parent>(*cEntity);
        if (parentComponent != nullptr) {
            cEntity = &parentComponent->parent;
        } else {
            cEntity = nullptr;
        }
    }

    return modelm;
}