#ifndef APP_HPP
#define APP_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "GlfwWindow.hpp"
#include "Components/Solution.hpp"

class App {
public:
    App();

    void run();
private:
    GlfwWindow window;
    entt::registry scene;

    void initExplodedViewTestScene();

    void addTestPuzzle();
    void addPuzzleFromModel();

    entt::entity addPiece(entt::entity puzzle, glm::vec3 position, Solution solution);
    entt::entity addBlock(entt::entity piece, glm::vec3 position, glm::vec3 color);
};

#endif //APP_HPP