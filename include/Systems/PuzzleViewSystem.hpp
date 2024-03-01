#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "GlfwWindow.hpp"

class PuzzleViewSystem {
public:
    static void init(entt::registry& scene, GlfwWindow& window);
    static void update();

private:
    // I'm not using smart pointers here since smart pointer functions don't have access to the private constructor
    static PuzzleViewSystem* puzzleViewSystem;

    entt::registry &scene;
    GlfwWindow& window;

    glm::vec2 prevMousePos;

    PuzzleViewSystem(entt::registry& scene, GlfwWindow& window);

    static void updateExplodedView();
    static void updatePuzzleRotation();

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
