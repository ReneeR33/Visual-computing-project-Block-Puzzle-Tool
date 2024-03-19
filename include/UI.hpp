#ifndef UI_HPP
#define UI_HPP

#include <entt/entt.hpp>
#include <list>
#include "glm/glm.hpp"

glm::vec2 UIEntityScreenPosition(entt::registry& scene, entt::entity uiEntity);

entt::entity addScrollView(entt::registry& scene, int layer, float bottom, float top, float left, float right, glm::vec2 position);
void addUIEntityToScrollView(entt::registry& scene, entt::entity& scrollView, entt::entity& uiEntity);
std::list<entt::entity> getScrollViewChildren(entt::registry& scene, entt::entity& scrollView);

#endif //UI_HPP