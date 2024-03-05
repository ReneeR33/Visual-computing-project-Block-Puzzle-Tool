#include "App.hpp"

#include "Components/Background.hpp"
#include "Components/Shader.hpp"
#include "Systems/Renderer.hpp"
#include "Systems/PuzzleViewSystem.hpp"
#include "DebugWindow.hpp"
#include "Components/Material.hpp"
#include "Components/ExplodedView.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/Puzzle.hpp"
#include "primitives.hpp"
#include "ModelLoader.hpp"

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
}

void App::run() {
    Renderer renderer;
    PuzzleViewSystem::init(scene, window);

    initExplodedViewTestScene();

    renderer.load(scene);
    DebugWindow debugWindow(window);

    while (!window.windowShouldClose()) {
        glfwPollEvents();
        PuzzleViewSystem::update();

        renderer.render(scene);
        debugWindow.render(scene);

        window.update();
    }
}

void App::initExplodedViewTestScene() {
    auto background = scene.create();
    ModelLoader loader = ModelLoader();
    scene.emplace<Background>(background, glm::vec3(0.15f, 0.15f, 0.17f));

    auto puzzle = scene.create();
    scene.emplace<Puzzle>(puzzle);
    scene.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(50.f),
                             glm::vec3(1.0f));
    scene.emplace<ExplodedView>(puzzle,0.0f);

    auto dirLight = scene.create();
    scene.emplace<DirLight>(dirLight,
                            glm::vec3(-0.4f, 0.14f, -1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f)
    );

    auto camera = scene.create();
    scene.emplace<Camera>(camera,
                          glm::vec3(0.0f, 0.0f, 6.0f),
                          glm::vec3(0.0f, 0.0f, -1.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f),
                          0.1f, 100.0f, 80.0f
    );

    auto& puzzleChildren = scene.emplace<Children>(puzzle);
    auto result = loader.LoadSolution("resources/data/");

    for (auto & block : result.pieces)
    {
        entt::entity piece = scene.create();
        scene.emplace<Model>(piece, block.model);
        scene.emplace<Material>(piece, block.material);
        scene.emplace<PuzzlePiece>(piece, block.piece);
        scene.emplace<Shader>(piece, "shaders/shader.vert", "shaders/shader.frag");
        scene.emplace<Transform>(piece,
                                    glm::vec3(block.piece.initialPosition),
                                    glm::vec3(block.piece.initialRotation),
                                    glm::vec3(1.0f, 1.0f, 1.0f)
        );

        puzzleChildren.children.push_front(piece);
        scene.emplace<Parent>(piece, puzzle);
    }
}
