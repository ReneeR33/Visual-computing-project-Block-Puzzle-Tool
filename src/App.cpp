#include "App.hpp"

#include "Components/Background.hpp"
#include "Components/Shader.hpp"
#include "Systems/Renderer.hpp"
#include "Systems/PuzzleViewSystem.hpp"
#include "Systems/PieceViewSystem.hpp"
#include "Systems/UISystem.hpp"
#include "DebugWindow.hpp"
#include "Components/Material.hpp"
#include "Components/ExplodedView.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Parent.hpp"
#include "Components/Children.hpp"
#include "Components/Puzzle.hpp"
#include "Components/UICanvas.hpp"
#include "Components/PiecesView.hpp"
#include "Components/Transform2D.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/Fill2D.hpp"
#include "Components/UIScene.hpp"
#include "Components/ScrollView.hpp"
#include "InputSystem.hpp"
#include "primitives.hpp"
#include "ModelLoader.hpp"
#include "UI.hpp"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

#define PIECE_VIEW_WIDTH WINDOW_WIDTH / 5

#define LOAD_TEST_PUZZLE

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
}

void App::run() {
    InputSystem::init(window);

    Renderer renderer;
    UISystem uiSystem;
    PuzzleViewSystem puzzleViewSystem(scene);
    PieceViewSystem pieceViewSystem(scene);

    initExplodedViewTestScene();

    renderer.load(scene);
    DebugWindow debugWindow(window);

    while (!window.windowShouldClose()) {
        InputSystem::update();

        uiSystem.update(scene);
        puzzleViewSystem.update();
        pieceViewSystem.update();

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

    auto puzzle = addTestPuzzle();

    // UI
    auto uiCanvas = scene.create();
    scene.emplace<UICanvas>(uiCanvas);
    scene.emplace<Transform2D>(uiCanvas, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    scene.emplace<Children>(uiCanvas);

    addPieceView(uiCanvas, puzzle);
}

entt::entity App::addTestPuzzle() {
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

    return puzzle;
}

entt::entity App::addPieceView(entt::entity canvas, entt::entity puzzle)
{
    auto& canvasChildren = scene.get<Children>(canvas);

    auto pieceView = scene.create();
    scene.emplace<Parent>(pieceView, canvas);
    canvasChildren.children.push_front(pieceView);
    scene.emplace<Transform2D>(pieceView, 
        glm::vec2(float(WINDOW_WIDTH) - float(PIECE_VIEW_WIDTH) / 2.0f, 
        float(WINDOW_HEIGHT) / 2.0f), 0.0f, 
        glm::vec2(1.0f)
    );
    auto& pieceViewComponent = scene.emplace<PiecesView>(pieceView);
    pieceViewComponent.puzzle = puzzle;

    scene.emplace<CanvasElement>(pieceView, 0,
        float(WINDOW_HEIGHT) / 2.0f, -float(WINDOW_HEIGHT) / 2.0f,
        -float(PIECE_VIEW_WIDTH) / 2.0f, float(PIECE_VIEW_WIDTH) / 2.0f
    );
    auto& pieceViewChildren = scene.emplace<Children>(pieceView);

    auto pieceViewBackground = scene.create();
    scene.emplace<Parent>(pieceViewBackground, pieceView);
    pieceViewChildren.children.push_front(pieceViewBackground);
    scene.emplace<CanvasElement>(pieceViewBackground, 0);
    scene.emplace<Transform2D>(pieceViewBackground, glm::vec2(0.0f), 0.0f, glm::vec3(1.0f));
    scene.emplace<Fill2D>(pieceViewBackground, glm::vec3(0.05f, 0.05f, 0.08f), float(PIECE_VIEW_WIDTH), float(WINDOW_HEIGHT));
    pieceViewComponent.background = pieceViewBackground;

    auto pieceViewScrollView = addScrollView(scene,
        1,
        -float(WINDOW_HEIGHT) / 2.0f, float(WINDOW_HEIGHT) / 2.0f,
        -float(PIECE_VIEW_WIDTH) / 2.0f, float(PIECE_VIEW_WIDTH) / 2.0f,
        glm::vec2(0.0f)
    );
    scene.emplace<Parent>(pieceViewScrollView, pieceView);
    pieceViewChildren.children.push_front(pieceViewScrollView);
    pieceViewComponent.scrollView = pieceViewScrollView;

    const float pieceViewSinglePieceViewWidth = float(WINDOW_WIDTH) / 7.0f;
    const float pieceViewSinglePieceViewHeight = float(WINDOW_HEIGHT) / 3.0f;

    auto& puzzleChildren = scene.get<Children>(puzzle);

    int i = 0;
    for (auto pieceEntity : puzzleChildren.children) {
        auto& pieceChildren = scene.get<Children>(pieceEntity);

        auto pieceViewSinglePieceView = scene.create();
        scene.emplace<CanvasElement>(pieceViewSinglePieceView, 1,
            pieceViewSinglePieceViewHeight / 2.0f, -pieceViewSinglePieceViewHeight / 2.0f,
            -pieceViewSinglePieceViewWidth / 2.0f, pieceViewSinglePieceViewWidth / 2.0f
        );
        scene.emplace<Transform2D>(pieceViewSinglePieceView, 
            glm::vec2(0.0f, -(float(i) * (pieceViewSinglePieceViewHeight + 50.0f))), 
            0.0f, 
            glm::vec3(1.0f)
        );
        auto& pieceViewSinglePieceViewChildren = scene.emplace<Children>(pieceViewSinglePieceView);

        auto pieceViewSinglePieceViewBackGround = scene.create();
        scene.emplace<Parent>(pieceViewSinglePieceViewBackGround, pieceViewSinglePieceView);
        pieceViewSinglePieceViewChildren.children.push_front(pieceViewSinglePieceViewBackGround);
        scene.emplace<CanvasElement>(pieceViewSinglePieceViewBackGround, 0);
        scene.emplace<Transform2D>(pieceViewSinglePieceViewBackGround, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(1.0f));
        scene.emplace<Fill2D>(pieceViewSinglePieceViewBackGround, 
            glm::vec3(0.15f, 0.15f, 0.18f), 
            pieceViewSinglePieceViewWidth, 
            pieceViewSinglePieceViewHeight
        );

        auto pieceUIScene = scene.create();
        scene.emplace<Parent>(pieceUIScene, pieceViewSinglePieceView);
        pieceViewSinglePieceViewChildren.children.push_front(pieceUIScene);
        scene.emplace<CanvasElement>(pieceUIScene, 1);
        scene.emplace<Transform2D>(pieceUIScene, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(1.0f));
        auto& uiScene = scene.emplace<UIScene>(pieceUIScene);
        uiScene.width = pieceViewSinglePieceViewWidth;
        uiScene.height = pieceViewSinglePieceViewHeight;

        auto dirLight = uiScene.scene.create();
        uiScene.scene.emplace<DirLight>(dirLight,
                                glm::vec3(-0.4f, 0.14f, -1.0f),
                                glm::vec3(1.0f),
                                glm::vec3(1.0f),
                                glm::vec3(1.0f)
        );

        auto camera = uiScene.scene.create();
        uiScene.scene.emplace<Camera>(camera,
                            glm::vec3(0.0f, 0.0f, 5.0f),
                            glm::vec3(0.0f, 0.0f, -1.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            0.1f, 100.0f, 80.0f
        );

        auto uiScenePiece = uiScene.scene.create();
        uiScene.scene.emplace<Transform>(uiScenePiece, glm::vec3(0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(1.0f));
        auto& uiScenePieceChildren = uiScene.scene.emplace<Children>(uiScenePiece);

        for (auto block : pieceChildren.children) {
            auto uiScenePieceBlock = uiScene.scene.create();
            uiScene.scene.emplace<Parent>(uiScenePieceBlock, uiScenePiece);
            uiScene.scene.emplace<Model>(uiScenePieceBlock, primitives::cube);
            uiScene.scene.emplace<Shader>(uiScenePieceBlock, "shaders/phong/phong.vert", "shaders/phong/phong.frag");

            auto& transform = scene.get<Transform>(block);
            auto& material = scene.get<Material>(block);

            uiScene.scene.emplace<Material>(uiScenePieceBlock, material);
            uiScene.scene.emplace<Transform>(uiScenePieceBlock, transform);
        }

        addUIEntityToScrollView(scene, pieceViewScrollView, pieceViewSinglePieceView);

        i++;
    }

    return pieceView;
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
        scene.emplace<Shader>(piece, "shaders/phong/phong.vert", "shaders/phong/phong.frag");
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
    scene.emplace<Shader>(block, "shaders/phong/phong.vert", "shaders/phong/phong.frag");
    scene.emplace<Material>(block,
                            color,
                            glm::vec3(0.1f, 0.1f, 0.12f),
                            glm::vec3(0.0f),
                            1.0f
    );
    scene.emplace<Transform>(block,
                             glm::vec3(position),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f)
    );

    pieceChildren->children.push_front(block);
    scene.emplace<Parent>(block, piece);

    return block;
}