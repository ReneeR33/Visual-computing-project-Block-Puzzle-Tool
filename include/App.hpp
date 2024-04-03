#ifndef APP_HPP
#define APP_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "GlfwWindow.hpp"
#include "Scene.hpp"
#include "Components/Solution.hpp"
#include "PuzzleLoader.hpp"
#include "Systems/Renderer.hpp"

class App {
public:
    App();

    void run();
private:
    GlfwWindow window;
    Scene scene;
    PuzzleLoader loader;

    //void initExplodedViewTestScene();
    void resetScene();
    void initScene(Renderer &renderer, entt::entity puzzle);

    entt::entity addTestPuzzle();
    entt::entity addPieceView(entt::entity canvas, entt::entity puzzle);
    entt::entity addUI(entt::entity canvas);
    entt::entity addPuzzleFromModel(std::string path);

    entt::entity addPiece(entt::entity puzzle, glm::vec3 position, glm::vec3 color, Solution solution);
    entt::entity addBlock(entt::entity piece, glm::vec3 position);
};

#endif //APP_HPP