#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "GlfwWindow.hpp"
#include <entt/entt.hpp>
#include <vector>

class DebugWindow {
public:
    enum Action
    {
        none,
        load,
        save
    };

    DebugWindow(GlfwWindow& window);

    Action render(entt::registry& scene);

private:
    GlfwWindow& window;

    Action LoadFile();
    void CameraInfo(entt::registry& scene);
    void ObjectInfo(entt::registry& scene);
    void LightInfo(entt::registry& scene);
    void ExplodedViewInfo(entt::registry& scene);
    void SolutionInfo(entt::registry& scene);
};

#endif //DEBUG_WINDOW_HPP