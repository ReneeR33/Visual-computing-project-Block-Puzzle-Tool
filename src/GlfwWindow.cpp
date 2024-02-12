#include "GlfwWindow.hpp"

#include <stdexcept>


GlfwWindow::GlfwWindow(int width, int height, const std::string& name) 
    : width(width)
    , height(height)
    , name(name) 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // disable resizing for now
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        throw std::runtime_error("GlfwWindow::GlfwWindow: failed to create window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("GlfwWindow::GlfwWindow: Failed to initialize GLAD");
    }
}

GlfwWindow::~GlfwWindow() {
    glfwTerminate();
}

bool GlfwWindow::windowShouldClose() {
    return glfwWindowShouldClose(window);
}

void GlfwWindow::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}