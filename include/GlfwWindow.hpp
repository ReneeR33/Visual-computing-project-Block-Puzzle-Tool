#ifndef GLFW_WINDOW_HPP 
#define GLFW_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class GlfwWindow {
public:
    GlfwWindow(int width, int height, const std::string& name);
   ~GlfwWindow();

    GLFWwindow* getHandle() const { return this->window; }

    bool windowShouldClose();
    void update();
    int width;
    int height;
private:
    GLFWwindow *window;
    std::string name;
};

#endif //GLFW_WINDOW_HPP