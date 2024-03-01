#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

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

    PuzzleViewSystem(entt::registry& scene, GlfwWindow& window);

    static void updateExplodedView();

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
