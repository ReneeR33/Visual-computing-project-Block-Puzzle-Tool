#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "GlfwWindow.hpp"
#include "Scene.hpp"

class DebugWindow {
public:
    DebugWindow(GlfwWindow& window, Scene& scene);

    void render();

private:
    GlfwWindow& window;
    Scene& scene;
};

#endif //DEBUG_WINDOW_HPP