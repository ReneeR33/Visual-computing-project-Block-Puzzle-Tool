#include "Systems/UISystem.hpp"

#include "Components/ScrollView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/Fill2D.hpp"


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
    
        auto scrollBoxtTransform = scene.try_get<Transform2D>(scrollView.scrollBox);
        auto scrollBoxCanvasElement = scene.try_get<CanvasElement>(scrollView.scrollBox);
        if (scrollBoxtTransform != nullptr && scrollBoxCanvasElement != nullptr) {
            scrollBoxtTransform->position.y = scrollView.height / 2 - scrollBoxCanvasElement->top + scrollView.value;
        }

        auto indicatorTransform = scene.try_get<Transform2D>(scrollView.scrollIndicator);
        auto indicatorFill = scene.try_get<Fill2D>(scrollView.scrollIndicator);
        if (indicatorTransform != nullptr && indicatorFill != nullptr) {
            float indicatorOffset = (scrollView.value / scrollView.maxValue) * ((scrollView.height / indicatorFill->height) - 1) * indicatorFill->height;
            indicatorTransform->position.y = scrollView.height / 2 - (indicatorFill->height / 2) - indicatorOffset;
        }
    }
}