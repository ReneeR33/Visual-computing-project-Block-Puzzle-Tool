#ifndef UI_HPP
#define UI_HPP

#include <entt/entt.hpp>
#include "glm/glm.hpp"

entt::entity addScrollView(entt::registry& scene, int layer, float bottom, float top, float left, float right, glm::vec2 position);
void addUIEntityToScrollView(entt::registry& scene, entt::entity& scrollView, entt::entity& uiEntity);

#endif //UI_HPP