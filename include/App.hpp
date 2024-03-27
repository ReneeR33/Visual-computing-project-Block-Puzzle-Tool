#ifndef APP_HPP
#define APP_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "GlfwWindow.hpp"
#include "Scene.hpp"

class App {
public:
    App();

    void run();
private:
    GlfwWindow window;
    Scene scene;

    void initExplodedViewTestScene();

    entt::entity addTestPuzzle();
    entt::entity addPieceView(entt::entity canvas, entt::entity puzzle);
    entt::entity addPuzzleFromModel();

    entt::entity addPiece(entt::entity puzzle, glm::vec3 position, glm::vec3 color);
    entt::entity addBlock(entt::entity piece, glm::vec3 position);
};

#endif //APP_HPP