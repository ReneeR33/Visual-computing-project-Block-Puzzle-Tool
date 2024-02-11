#ifndef APP_HPP
#define APP_HPP

#include "GlfwWindow.hpp"

class App {
public:
    App();

    void run();
private:
    GlfwWindow window;
};

#endif //APP_HPP