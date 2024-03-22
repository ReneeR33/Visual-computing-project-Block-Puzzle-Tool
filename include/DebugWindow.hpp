#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "GlfwWindow.hpp"
#include <entt/entt.hpp>
#include <vector>

class DebugWindow {
public:
    DebugWindow(GlfwWindow& window);

    enum Action
    {
        none,
        load,
        save
    };

    Action render(entt::registry& scene);

private:
    GlfwWindow& window;

    Action LoadFile(entt::registry& scene);
    void CameraInfo(entt::registry& scene);
    void ObjectInfo(entt::registry& scene);
    void LightInfo(entt::registry& scene);
    void ExplodedViewInfo(entt::registry& scene);
    void SolutionInfo(entt::registry& scene);
};

#endif //DEBUG_WINDOW_HPP