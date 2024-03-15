#ifndef PIECE_VIEW_SYSTEM
#define PIECE_VIEW_SYSTEM

#include <entt/entt.hpp>
#include "GlfwWindow.hpp"

class PieceViewSystem {
public:
    static void init(entt::registry& scene, GlfwWindow& window);
    static void update();

private:
    static PieceViewSystem* pieceViewSystem;

    entt::registry &scene;
    GlfwWindow& window;

    PieceViewSystem(entt::registry& scene, GlfwWindow& window);

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //PIECE_VIEW_SYSTEM