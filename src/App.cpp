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

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

#define LOAD_TEST_PUZZLE

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
    scene.emplace<Background>(background, glm::vec3(0.15f, 0.15f, 0.17f));

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

#ifdef LOAD_TEST_PUZZLE
    addTestPuzzle();
#else
    addPuzzleFromModel();
#endif
}

void App::addTestPuzzle() {
    auto puzzle = scene.create();
    scene.emplace<Puzzle>(puzzle);
    scene.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.emplace<ExplodedView>(puzzle,0.0f);
    scene.emplace<Children>(puzzle);

    auto piece_1 = addPiece(puzzle, glm::vec3(-1.0f, 0.0f, -1.0f));
    auto color = glm::vec3(0.6f,0.0f,0.0f);
    addBlock(piece_1, glm::vec3(0.0f,0.0f,0.0f), color);
    addBlock(piece_1, glm::vec3(1.0f,0.0f,0.0f), color);
    addBlock(piece_1, glm::vec3(0.0f,1.0f,0.0f), color);
    addBlock(piece_1, glm::vec3(0.0f,0.0f,1.0f), color);
    addBlock(piece_1, glm::vec3(0.0f,-1.0f,0.0f), color);

    auto piece_2 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 0.0f));
    color = glm::vec3(0.3f, 0.0f, 0.3f);
    addBlock(piece_2, glm::vec3(0.0f,0.0f,0.0f), color);
    addBlock(piece_2, glm::vec3(0.0f,1.0f,0.0f), color);
    addBlock(piece_2, glm::vec3(0.0f,-1.0f,0.0f), color);

    auto piece_3 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, -1.0f));
    color = glm::vec3(0.3f, 0.0f, 0.6f);
    addBlock(piece_3, glm::vec3(0.0f,0.0f,0.0f), color);
    addBlock(piece_3, glm::vec3(0.0f,1.0f,0.0f), color);
    addBlock(piece_3, glm::vec3(-1.0f,1.0f,0.0f), color);
    addBlock(piece_3, glm::vec3(-1.0f,-1.0f,0.0f), color);
    addBlock(piece_3, glm::vec3(0.0f,-1.0f,0.0f), color);

    auto piece_4 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 1.0f));
    color = glm::vec3(0.6f, 0.6f, 0.0f);
    addBlock(piece_4, glm::vec3(0.0f,0.0f,0.0f), color);
    addBlock(piece_4, glm::vec3(0.0f,1.0f,0.0f), color);
    addBlock(piece_4, glm::vec3(0.0f,-1.0f,0.0f), color);
    addBlock(piece_4, glm::vec3(-1.0f,0.0f,0.0f), color);
    addBlock(piece_4, glm::vec3(1.0f,0.0f,0.0f), color);

    auto piece_5 = addPiece(puzzle, glm::vec3(-1.0f, 1.0f, 0.5f));
    color = glm::vec3(0.3f, 0.3f, 0.6f);
    addBlock(piece_5, glm::vec3(0.0f,0.0f,0.5f), color);
    addBlock(piece_5, glm::vec3(0.0f,0.0f,-0.5f), color);

    auto piece_6 = addPiece(puzzle, glm::vec3(-1.0f, -1.0f, 0.5f));
    color = glm::vec3(0.3f, 0.6f, 0.0f);
    addBlock(piece_6, glm::vec3(0.0f,0.0f,0.5f), color);
    addBlock(piece_6, glm::vec3(0.0f,0.0f,-0.5f), color);

    auto piece_7 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, 0.0f));
    color = glm::vec3(0.3f, 0.6f, 0.3f);
    addBlock(piece_7, glm::vec3(0.0f,0.0f,0.0f), color);
    addBlock(piece_7, glm::vec3(0.0f,1.0f,0.0f), color);
    addBlock(piece_7, glm::vec3(0.0f,-1.0f,0.0f), color);
    addBlock(piece_7, glm::vec3(0.0f,1.0f,1.0f), color);
    addBlock(piece_7, glm::vec3(0.0f,-1.0f,1.0f), color);
}

void App::addPuzzleFromModel() {
    auto puzzle = scene.create();
    scene.emplace<Puzzle>(puzzle);
    scene.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(50.f),
                             glm::vec3(1.0f));
    scene.emplace<ExplodedView>(puzzle,0.0f);

    auto& puzzleChildren = scene.emplace<Children>(puzzle);

    ModelLoader loader = ModelLoader();
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

entt::entity App::addPiece(entt::entity puzzle, glm::vec3 position) {
    auto puzzleChildren = scene.try_get<Children>(puzzle);
    if (puzzleChildren == nullptr) {
        throw std::runtime_error("App::addPiece: cannot add piece to entity without children");
    }

    auto piece = scene.create();
    scene.emplace<Parent>(piece, puzzle);
    scene.emplace<Transform>(piece,
                             position,
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.emplace<Children>(piece);

    puzzleChildren->children.push_front(piece);
    scene.emplace<PuzzlePiece>(piece, position);

    return piece;
}


entt::entity App::addBlock(entt::entity piece, glm::vec3 position, glm::vec3 color) {
    auto pieceChildren = scene.try_get<Children>(piece);
    if (pieceChildren == nullptr) {
        throw std::runtime_error("App::addBlock: cannot add block to entity without children");
    }

    auto block = scene.create();

    scene.emplace<Model>(block, primitives::cube);
    scene.emplace<Shader>(block, "shaders/shader.vert", "shaders/shader.frag");
    scene.emplace<Material>(block,
                            color,
                            glm::vec3(0.1f, 0.1f, 0.12f),
                            glm::vec3(0.0f),
                            1.0f
    );
    scene.emplace<Transform>(block,
                             glm::vec3(position),
                             glm::vec3(0.0f),
                             glm::vec3(0.98f)
    );

    pieceChildren->children.push_front(block);
    scene.emplace<Parent>(block, piece);

    return block;
}