#include "InputSystem.hpp"

#include <stdexcept>

InputSystem* InputSystem::inputSystem = nullptr;

InputSystem::InputSystem(GlfwWindow& window) 
    : window(window) {
}

void InputSystem::init(GlfwWindow& window) {
    delete inputSystem;
    inputSystem = new InputSystem(window);
}

void InputSystem::update() {
    glfwPollEvents();
}

template<class T>
entt::sink<entt::sigh<void (T &), std::allocator<void>>> InputSystem::event() {
    if (inputSystem == nullptr) {
        throw std::runtime_error("InputSystem::event: inputSystem is not initialized");
    }

    return inputSystem->dispatcher.sink<T>();
}

void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (inputSystem == nullptr) {
        return;
    }

    inputSystem->dispatcher.trigger(ScrollEvent{
        .xoffset = xoffset,
        .yoffset = yoffset
    });
}

void InputSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (inputSystem == nullptr) {
        return;
    }

    inputSystem->dispatcher.trigger(MouseButtonCallBackEvent{
        .button = button,
        .action = action,
        .mods = mods
    });
}