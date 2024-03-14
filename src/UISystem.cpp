#include "Systems/UISystem.hpp"

#include "Components/ScrollView.hpp"
#include "Components/Transform.hpp"


void UISystem::update(entt::registry &scene) {
    updateScrollView(scene);
}

void UISystem::updateScrollView(entt::registry &scene) {
    for (auto [entity, scrollView] : scene.view<ScrollView>().each()) {
        if (scrollView.value < scrollView.minValue) {
            scrollView.value = scrollView.minValue;
        } else if (scrollView.value > scrollView.maxValue) {
            scrollView.value = scrollView.maxValue;
        }

        auto transform = scene.try_get<Transform>(entity);
        if (transform != nullptr) {
            transform->position.y = scrollView.value;
        }
    }
}