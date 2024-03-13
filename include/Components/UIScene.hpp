#ifndef UI_SCENE_HPP
#define UI_SCENE_HPP

#include <entt/entt.hpp>
#include <vector>

struct UIScene {
    float width;
    float height;
    entt::registry scene;
};

#endif //UI_SCENE_HPP