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
    int getWidth() const { return this->width; }
    int getHeight() const { return this->height; }

    bool windowShouldClose();
    void update();
private:
    GLFWwindow *window;
    int width;
    int height;
    std::string name;
};

#endif //GLFW_WINDOW_HPP