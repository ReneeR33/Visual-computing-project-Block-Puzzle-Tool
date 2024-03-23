#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

glm::mat4 getModelMatrix(entt::registry& scene, entt::entity worldEntity);

#endif //ENTITY_HPP