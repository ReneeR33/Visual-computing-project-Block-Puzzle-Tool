#ifndef UI_SCENE_HPP
#define UI_SCENE_HPP

#include <entt/entt.hpp>
#include <vector>

struct UIScene {
    float width;
    float height;
    std::vector<entt::entity> entities;
};

#endif //UI_SCENE_HPP