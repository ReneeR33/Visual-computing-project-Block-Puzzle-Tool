#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <entt/entt.hpp>

struct ScrollView {
    float value;
    float minValue;
    float maxValue;

    entt::entity scrollBox;
    entt::entity scrollIndicator;
};

#endif //SCROLL_VIEW_HPP