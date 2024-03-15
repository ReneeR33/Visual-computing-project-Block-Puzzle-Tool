#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include <entt/entt.hpp>
#include "GlfwWindow.hpp"

class InputSystem {
public:
    struct ScrollEvent {
        double xoffset;
        double yoffset;
    };

    struct MouseButtonCallBackEvent {
        int button;
        int action; 
        int mods;
    };

    static void init(GlfwWindow& window);
    static void update();

    static void getCursorPos(double& xpos, double& ypos);

    template<typename T>
    static entt::sink<entt::sigh<void (T &), std::allocator<void>>> event() {
        if (inputSystem == nullptr) {
            throw std::runtime_error("InputSystem::event: inputSystem is not initialized");
        }

        return inputSystem->dispatcher.sink<T>();
    }

private:
    static InputSystem* inputSystem;

    GlfwWindow& window;
    entt::dispatcher dispatcher;

    InputSystem(GlfwWindow& window);

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif //INPUT_SYSTEM_HPP