#include "UI.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/Children.hpp"
#include "Components/ScrollView.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/Transform2D.hpp"
#include "Components/Fill2D.hpp"
#include "Components/Button.hpp"

#define SCROLL_INDICATOR_WIDTH 10.0f
#define SCROLL_INDICATOR_OFFSET 3.0f

glm::vec2 UIEntityScreenPosition(entt::registry& scene, entt::entity uiEntity) {
    glm::mat4 model(1.0f);
    
    entt::entity* cEntity = &uiEntity;
    while (cEntity != nullptr) {
        auto transform = scene.try_get<Transform2D>(*cEntity);

        if (transform != nullptr) {
            glm::mat4 localModel(1.0f);
            localModel = glm::translate(localModel, glm::vec3(transform->position, 0.0f));
            model = localModel * model;
        }

        auto parent = scene.try_get<Parent>(*cEntity);
        if (parent != nullptr) {
            cEntity = &parent->parent;
        } else {
            cEntity = nullptr;
        }
    }

    return glm::vec2(model[3]);
}

entt::entity addButton(entt::registry& scene, int layer, float bottom, float top, float left, float right, glm::vec2 position) {
    auto buttonEntity = scene.create();

    scene.emplace<CanvasElement>(buttonEntity, layer,
        top, bottom, left, right
    );
    scene.emplace<Transform2D>(buttonEntity, position, 0.0f, glm::vec2(1.0f));
    auto& button = scene.emplace<Button>(buttonEntity);
    
    // TODO: configure this somewhere else?
    button.color = glm::vec3(0.4f, 0.4f, 0.4f);
    button.hoverColor = glm::vec3(0.5f, 0.5f, 0.5f);
    button.pressColor = glm::vec3(0.6f, 0.6f, 0.6f);

    auto& buttonChildren = scene.emplace<Children>(buttonEntity);

    auto background = scene.create();
    scene.emplace<Parent>(background, buttonEntity);
    buttonChildren.children.push_back(background);
    scene.emplace<CanvasElement>(background, 1,
        top, bottom, left, right
    );
    scene.emplace<Transform2D>(background, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    scene.emplace<Fill2D>(background,
        button.color,
        right - left, top - bottom,
        10.0f
    );
    button.background = background;

    return buttonEntity;
}

entt::entity addScrollView(entt::registry& scene, int layer, float bottom, float top, float left, float right, glm::vec2 position) {
    auto scrollViewEntity = scene.create();

    scene.emplace<CanvasElement>(scrollViewEntity, layer,
        top, bottom, left, right
    );
    scene.emplace<Transform2D>(scrollViewEntity, position, 0.0f, glm::vec2(1.0f));
    auto& scrollViewComponent = scene.emplace<ScrollView>(scrollViewEntity, 0.0f);
    auto& scrollViewChildren = scene.emplace<Children>(scrollViewEntity);

    auto scrollBox = scene.create();
    scene.emplace<Parent>(scrollBox, scrollViewEntity);
    scrollViewChildren.children.push_front(scrollBox);
    auto& scrollBoxElement = scene.emplace<CanvasElement>(scrollBox, 0,
        0.0f, 0.0f, left, right
    );
    scene.emplace<Transform2D>(scrollBox, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    scene.emplace<Children>(scrollBox);
    scrollViewComponent.scrollBox = scrollBox;

    auto scrollIndicator = scene.create();
    scene.emplace<Parent>(scrollIndicator, scrollViewEntity);
    scrollViewChildren.children.push_front(scrollIndicator);
    scene.emplace<CanvasElement>(scrollIndicator, 1,
        0.0f, 0.0f, -SCROLL_INDICATOR_WIDTH / 2, SCROLL_INDICATOR_WIDTH / 2
    );
    scene.emplace<Fill2D>(scrollIndicator,
        glm::vec3(0.83f, 0.83f, 0.84f),
        SCROLL_INDICATOR_WIDTH, 0.0f,
        SCROLL_INDICATOR_WIDTH / 2
    );
    scene.emplace<Transform2D>(scrollIndicator,
        glm::vec2(right - SCROLL_INDICATOR_WIDTH / 2 - SCROLL_INDICATOR_OFFSET, top),
        0.0f,
        glm::vec2(1.0f)
    );
    scrollViewComponent.scrollIndicator = scrollIndicator;

    return scrollViewEntity;
}

void addUIEntityToScrollView(entt::registry& scene, entt::entity& scrollView, entt::entity& uiEntity) {
    auto scrollViewComponent = scene.try_get<ScrollView>(scrollView);
    if (scrollViewComponent == nullptr) {
        return;
    }

    auto scrollBox = scrollViewComponent->scrollBox;
    auto& scrollBoxChildren = scene.get<Children>(scrollBox);
    auto& scrollBoxCanvas = scene.get<CanvasElement>(scrollBox);
    auto& scrollBoxTransform = scene.get<Transform2D>(scrollBox);
    auto& uiEntityCanvas = scene.get<CanvasElement>(uiEntity);
    auto& uiEntityTransform = scene.get<Transform2D>(uiEntity);
    auto& scrollViewCanvas = scene.get<CanvasElement>(scrollView);

    scene.emplace<Parent>(uiEntity, scrollBox);
    scrollBoxChildren.children.push_front(uiEntity);

    // ignore rotations/scaling, they're not going to be used inside this application anyway...
    auto uiEntityTop = uiEntityCanvas.top + uiEntityTransform.position.y;
    auto uiEntityBottom = uiEntityCanvas.bottom + uiEntityTransform.position.y;

    if (uiEntityTop > scrollBoxCanvas.top) {
        scrollBoxCanvas.top = uiEntityTop;
        scrollBoxTransform.position.y = scrollViewCanvas.top - scrollBoxCanvas.top + scrollViewComponent->value;
    }
    if (uiEntityBottom < scrollBoxCanvas.bottom) {
        scrollBoxCanvas.bottom = uiEntityBottom;
    }

    auto scrollViewHeight = scrollViewCanvas.top - scrollViewCanvas.bottom;
    auto scrollBoxHeight = scrollBoxCanvas.top - scrollBoxCanvas.bottom;

    // scroll box fits inside scroll view
    scrollViewComponent->maxValue = scrollBoxHeight - scrollViewHeight;
    if (scrollViewComponent->maxValue < 0.0f) {
        scrollViewComponent->maxValue = 0.0f;
        return;
    }

    auto indicator = scrollViewComponent->scrollIndicator;
    auto& indicatorTransform = scene.get<Transform2D>(indicator);
    auto& indicatorCanvas = scene.get<CanvasElement>(indicator);
    auto& indicatorFill = scene.get<Fill2D>(indicator);

    indicatorFill.height = scrollViewHeight * (scrollViewHeight / scrollBoxHeight);
    indicatorCanvas.top = indicatorFill.height / 2;
    indicatorCanvas.bottom = -indicatorCanvas.top;

    indicatorTransform.position.y = scrollViewCanvas.top - indicatorCanvas.top;
    indicatorTransform.position.y -= (scrollViewComponent->value / scrollViewComponent->maxValue) * (scrollViewHeight - indicatorFill.height);
}

std::list<entt::entity> getScrollViewChildren(entt::registry& scene, entt::entity& scrollView) {
    auto& scrollViewComponent = scene.get<ScrollView>(scrollView);
    return scene.get<Children>(scrollViewComponent.scrollBox).children;
}