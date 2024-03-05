#ifndef APP_HPP
#define APP_HPP

#include <entt/entt.hpp>
#include "GlfwWindow.hpp"

class App {
public:
    App();

    void run();
private:
    GlfwWindow window;
    entt::registry scene;

    void initExplodedViewTestScene();

    void addTestPuzzle();
    void addPuzzleFromModel();
};

#endif //APP_HPP