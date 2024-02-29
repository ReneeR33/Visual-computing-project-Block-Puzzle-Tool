#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "GlfwWindow.hpp"
#include <entt/entt.hpp>
#include <vector>

class DebugWindow {
public:
    DebugWindow(GlfwWindow& window);

    void render(entt::registry& scene);

private:
    GlfwWindow& window;

    void CameraInfo(entt::registry& scene);
    void ObjectInfo(entt::registry& scene);
    void LightInfo(entt::registry& scene);
    void ExplodedViewInfo(entt::registry& scene);
};

#endif //DEBUG_WINDOW_HPP