#ifndef PUZZLE_VIEW_SYSTEM_HPP
#define PUZZLE_VIEW_SYSTEM_HPP

#include "glm/glm.hpp"
#include <entt/entt.hpp>
#include "InputSystem.hpp"

class PuzzleViewSystem {
public:
    PuzzleViewSystem(entt::registry& scene);
    //static void init(entt::registry& scene, GlfwWindow& window);
    //static void update();
    void update();

private:
    // I'm not using smart pointers here since smart pointer functions don't have access to the private constructor
    // static PuzzleViewSystem* puzzleViewSystem;

    entt::registry &scene;
    // GlfwWindow& window;

    glm::vec2 prevMousePos;

    // PuzzleViewSystem(entt::registry& scene, GlfwWindow& window);

    //static void updateExplodedView();
    void updateExplodedView();
    //static void updatePuzzleRotation();
    void updatePuzzleRotation();

    //static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void scrollCallback(InputSystem::ScrollEvent scrollEvent);
    //static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseButtonCallback(InputSystem::MouseButtonCallBackEvent mouseButtonCallbackEvent);
};

#endif //PUZZLE_VIEW_SYSTEM_HPP
