#ifndef DEBUG_WINDOW_HPP
#define DEBUG_WINDOW_HPP

#include "GlfwWindow.hpp"
#include "Scene.hpp"
#include <vector>

class DebugWindow {
public:
    DebugWindow(GlfwWindow& window, Scene& scene);

    void render();

private:
    struct SceneModel {
        std::string name;
        Model* model;
    };

    GlfwWindow& window;
    Scene& scene;

    // using a vector for the scene models since we need to access them by index
    std::vector<SceneModel> sceneModelsVector;

    void ObjectInfo();
};

#endif //DEBUG_WINDOW_HPP