#include "InputSystem.hpp"

#include <stdexcept>

InputSystem* InputSystem::inputSystem = nullptr;

InputSystem::InputSystem(GlfwWindow& window) 
    : window(window) {
}

void InputSystem::init(GlfwWindow& window) {
    if (inputSystem == nullptr) {
        glfwSetScrollCallback(window.getHandle(), scrollCallback);
        glfwSetMouseButtonCallback(window.getHandle(), mouseButtonCallback);
    }

    delete inputSystem;
    inputSystem = new InputSystem(window);
}

void InputSystem::update() {
    glfwPollEvents();
}

void InputSystem::getCursorPos(double& xpos, double& ypos) {
    if (inputSystem == nullptr) {
        throw std::runtime_error("InputSystem::getCursorPos: inputSystem is not initialized");
    }

    auto& window = inputSystem->window;

    glfwGetCursorPos(window.getHandle(), &xpos, &ypos);
    ypos = window.getHeight() - ypos;
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