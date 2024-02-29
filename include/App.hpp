#ifndef APP_HPP
#define APP_HPP

#include <entt/entt.hpp>
#include "GlfwWindow.hpp"

class App {
public:
    App();

    void initTestScene();

    void run();
private:
    GlfwWindow window;
    entt::registry scene;
};

#endif //APP_HPP