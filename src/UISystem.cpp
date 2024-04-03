#include "Systems/UISystem.hpp"

#include "Components/ScrollView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/Fill2D.hpp"
#include "Components/Button.hpp"
#include "UI.hpp"

#include <iostream>

UISystem::UISystem(entt::registry& scene) 
    : scene(scene) {
    InputSystem::event<InputSystem::MouseButtonCallBackEvent>().connect<&UISystem::mouseButtonCallback>(*this);
}

void UISystem::update() {
    updateScrollView();
    updateButtons();
}

void UISystem::updateScrollView() {
    for (auto [entity, scrollView] : scene.view<ScrollView>().each()) {
        auto& scrollViewCanvas = scene.get<CanvasElement>(entity);

        auto scrollViewHeight = scrollViewCanvas.top - scrollViewCanvas.bottom;

        if (scrollView.value < scrollView.minValue) {
            scrollView.value = scrollView.minValue;
        } else if (scrollView.value > scrollView.maxValue) {
            scrollView.value = scrollView.maxValue;
        }
    
        auto scrollBoxtTransform = scene.try_get<Transform2D>(scrollView.scrollBox);
        auto scrollBoxCanvasElement = scene.try_get<CanvasElement>(scrollView.scrollBox);
        if (scrollBoxtTransform != nullptr && scrollBoxCanvasElement != nullptr) {
            scrollBoxtTransform->position.y = scrollViewCanvas.top - scrollBoxCanvasElement->top + scrollView.value;
        }

        auto indicatorTransform = scene.try_get<Transform2D>(scrollView.scrollIndicator);
        auto indicatorCanvas = scene.try_get<CanvasElement>(scrollView.scrollIndicator);
        if (indicatorTransform != nullptr && indicatorCanvas != nullptr) {
            auto indicatorHeight = indicatorCanvas->top - indicatorCanvas->bottom;
            float indicatorOffset = (scrollView.value / scrollView.maxValue) * (scrollViewHeight - indicatorHeight);
            indicatorTransform->position.y = scrollViewCanvas.top - indicatorCanvas->top - indicatorOffset;
        }
    }
}

void UISystem::updateButtons() {
    for (auto [entity, button] : scene.view<Button>().each()) {
        // do something
    }
}

void UISystem::mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent) {
    if (mouseButtonCallbackEvent.button == GLFW_MOUSE_BUTTON_LEFT && mouseButtonCallbackEvent.action == GLFW_PRESS) {
        double xpos, ypos;
        InputSystem::getCursorPos(xpos, ypos);

        for (auto [entity, button] : scene.view<Button>().each()) {
            auto& canvas = scene.get<CanvasElement>(entity);
            auto buttonScreenPos = UIEntityScreenPosition(scene, entity);
            
            if ((xpos > buttonScreenPos.x + canvas.left && xpos < buttonScreenPos.x + canvas.right) &&
                (ypos > buttonScreenPos.y + canvas.bottom && ypos < buttonScreenPos.y + canvas.top)) {
                std::cout << "press btn\n";
                button.buttonPressEventSignal.publish();
            }
        }
    }
}