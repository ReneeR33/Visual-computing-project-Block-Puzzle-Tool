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

int InputSystem::getMouseButton(int button) {
    if (inputSystem == nullptr) {
        throw std::runtime_error("InputSystem::getMouseButton: inputSystem is not initialized");
    }

    auto& window = inputSystem->window;

    return glfwGetMouseButton(window.getHandle(), button);
}

void InputSystem::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window; //silence warning
    if (inputSystem == nullptr) {
        return;
    }

    inputSystem->dispatcher.trigger(ScrollEvent{
        .xoffset = xoffset,
        .yoffset = yoffset
    });
}

void InputSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)window; //silence warning 
    if (inputSystem == nullptr) {
        return;
    }

    inputSystem->dispatcher.trigger(MouseButtonCallBackEvent{
        .button = button,
        .action = action,
        .mods = mods
    });
}