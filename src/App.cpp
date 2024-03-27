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
#include "Components/SinglePieceView.hpp"
#include "Components/BoundingBox.hpp"
#include "InputSystem.hpp"
#include "primitives.hpp"
#include "PuzzleLoader.hpp"
#include "UI.hpp"
#include "entity.hpp"

#include "ModelLoader.hpp"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

#define PIECE_VIEW_WIDTH WINDOW_WIDTH / 5

#define LOAD_TEST_PUZZLE

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
    ModelLoader modelLoader;
    modelLoader.loadModel(scene, "resources/models/wooden-cube/wooden-cube.obj");
    scene.models["cube"] = std::make_unique<ModelData>(primitives::cube_data);
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

        uiSystem.update(scene.registry);
        puzzleViewSystem.update();
        pieceViewSystem.update();

        renderer.render(scene.registry);
        debugWindow.render(scene.registry);

        window.update();
    }
}

void App::initExplodedViewTestScene() {
    auto background = scene.registry.create();
    scene.registry.emplace<Background>(background, glm::vec3(0.15f, 0.15f, 0.17f));

    auto dirLight = scene.registry.create();
    scene.registry.emplace<DirLight>(dirLight,
                            glm::vec3(-0.4f, 0.14f, -1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f)
    );

    auto camera = scene.registry.create();
    scene.registry.emplace<Camera>(camera,
                          glm::vec3(0.0f, 0.0f, 7.0f),
                          glm::vec3(0.0f, 0.0f, -1.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f),
                          0.1f, 100.0f, 70.0f
    );

    entt::entity puzzle;

    #ifndef LOAD_TEST_PUZZLE
    puzzle = addPuzzleFromModel();
    #else
    puzzle = addTestPuzzle();
    #endif

    // UI
    auto uiCanvas = scene.registry.create();
    scene.registry.emplace<UICanvas>(uiCanvas);
    scene.registry.emplace<Transform2D>(uiCanvas, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    scene.registry.emplace<Children>(uiCanvas);

    addPieceView(uiCanvas, puzzle);
}

entt::entity App::addTestPuzzle() {
    auto puzzle = scene.registry.create();
    scene.registry.emplace<Puzzle>(puzzle);
    scene.registry.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.registry.emplace<ExplodedView>(puzzle,0.0f);
    scene.registry.emplace<Children>(puzzle);

    auto color = glm::vec3(0.6f,0.0f,0.0f);
    auto piece_1 = addPiece(puzzle, glm::vec3(-1.0f, 0.0f, -1.0f), color);
    addBlock(piece_1, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_1, glm::vec3(1.0f,0.0f,0.0f));
    addBlock(piece_1, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_1, glm::vec3(0.0f,0.0f,1.0f));
    addBlock(piece_1, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.3f, 0.0f, 0.3f);
    auto piece_2 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 0.0f), color);
    addBlock(piece_2, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_2, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_2, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.3f, 0.0f, 0.6f);
    auto piece_3 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, -1.0f), color);
    addBlock(piece_3, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_3, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_3, glm::vec3(-1.0f,1.0f,0.0f));
    addBlock(piece_3, glm::vec3(-1.0f,-1.0f,0.0f));
    addBlock(piece_3, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.6f, 0.6f, 0.0f);
    auto piece_4 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 1.0f), color);
    addBlock(piece_4, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_4, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_4, glm::vec3(0.0f,-1.0f,0.0f));
    addBlock(piece_4, glm::vec3(-1.0f,0.0f,0.0f));
    addBlock(piece_4, glm::vec3(1.0f,0.0f,0.0f));

    color = glm::vec3(0.3f, 0.3f, 0.6f);
    auto piece_5 = addPiece(puzzle, glm::vec3(-1.0f, 1.0f, 0.5f), color);
    addBlock(piece_5, glm::vec3(0.0f,0.0f,0.5f));
    addBlock(piece_5, glm::vec3(0.0f,0.0f,-0.5f));

    color = glm::vec3(0.3f, 0.6f, 0.0f);
    auto piece_6 = addPiece(puzzle, glm::vec3(-1.0f, -1.0f, 0.5f), color);
    addBlock(piece_6, glm::vec3(0.0f,0.0f,0.5f));
    addBlock(piece_6, glm::vec3(0.0f,0.0f,-0.5f));

    color = glm::vec3(0.3f, 0.6f, 0.3f);
    auto piece_7 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, 0.0f), color);
    addBlock(piece_7, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_7, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_7, glm::vec3(0.0f,-1.0f,0.0f));
    addBlock(piece_7, glm::vec3(0.0f,1.0f,1.0f));
    addBlock(piece_7, glm::vec3(0.0f,-1.0f,1.0f));

    return puzzle;
}

entt::entity App::addPieceView(entt::entity canvas, entt::entity puzzle)
{
    auto& canvasChildren = scene.registry.get<Children>(canvas);

    auto pieceView = scene.registry.create();
    scene.registry.emplace<Parent>(pieceView, canvas);
    canvasChildren.children.push_front(pieceView);
    scene.registry.emplace<Transform2D>(pieceView, 
        glm::vec2(float(WINDOW_WIDTH) - float(PIECE_VIEW_WIDTH) / 2.0f, 
        float(WINDOW_HEIGHT) / 2.0f), 0.0f, 
        glm::vec2(1.0f)
    );
    auto& pieceViewComponent = scene.registry.emplace<PiecesView>(pieceView);
    pieceViewComponent.puzzle = puzzle;
    scene.registry.emplace<CanvasElement>(pieceView, 0,
        float(WINDOW_HEIGHT) / 2.0f, -float(WINDOW_HEIGHT) / 2.0f,
        -float(PIECE_VIEW_WIDTH) / 2.0f, float(PIECE_VIEW_WIDTH) / 2.0f
    );
    auto& pieceViewChildren = scene.registry.emplace<Children>(pieceView);

    auto pieceViewBackground = scene.registry.create();
    scene.registry.emplace<Parent>(pieceViewBackground, pieceView);
    pieceViewChildren.children.push_front(pieceViewBackground);
    scene.registry.emplace<CanvasElement>(pieceViewBackground, 0);
    scene.registry.emplace<Transform2D>(pieceViewBackground, glm::vec2(0.0f), 0.0f, glm::vec3(1.0f));
    scene.registry.emplace<Fill2D>(pieceViewBackground, glm::vec3(0.05f, 0.05f, 0.08f), float(PIECE_VIEW_WIDTH), float(WINDOW_HEIGHT));
    pieceViewComponent.background = pieceViewBackground;

    auto pieceViewScrollView = addScrollView(scene.registry,
        1,
        -float(WINDOW_HEIGHT) / 2.0f, float(WINDOW_HEIGHT) / 2.0f,
        -float(PIECE_VIEW_WIDTH) / 2.0f, float(PIECE_VIEW_WIDTH) / 2.0f,
        glm::vec2(0.0f)
    );
    scene.registry.emplace<Parent>(pieceViewScrollView, pieceView);
    pieceViewChildren.children.push_front(pieceViewScrollView);
    pieceViewComponent.scrollView = pieceViewScrollView;

    const float pieceViewSinglePieceViewWidth = float(WINDOW_WIDTH) / 7.0f;
    const float pieceViewSinglePieceViewHeight = float(WINDOW_HEIGHT) / 3.0f;

    auto& puzzleChildren = scene.registry.get<Children>(puzzle);

    int i = 0;
    for (auto pieceEntity : puzzleChildren.children) {
        auto& pieceChildren = scene.registry.get<Children>(pieceEntity);

        auto pieceViewSinglePieceView = scene.registry.create();
        auto& singlePieceViewComponent = scene.registry.emplace<SinglePieceView>(pieceViewSinglePieceView,
            glm::vec3(0.15f, 0.15f, 0.18f),
            glm::vec3(0.22f, 0.22f, 0.25f),
            pieceEntity
        );
        scene.registry.emplace<CanvasElement>(pieceViewSinglePieceView, 1,
            pieceViewSinglePieceViewHeight / 2.0f, -pieceViewSinglePieceViewHeight / 2.0f,
            -pieceViewSinglePieceViewWidth / 2.0f, pieceViewSinglePieceViewWidth / 2.0f
        );
        //TODO: store offset somewhere/make offset specific?
        scene.registry.emplace<Transform2D>(pieceViewSinglePieceView, 
            glm::vec2(0.0f, -(float(i) * (pieceViewSinglePieceViewHeight + 50.0f))), 
            0.0f, 
            glm::vec3(1.0f)
        );
        auto& pieceViewSinglePieceViewChildren = scene.registry.emplace<Children>(pieceViewSinglePieceView);

        auto pieceViewSinglePieceViewBackGround = scene.registry.create();
        scene.registry.emplace<Parent>(pieceViewSinglePieceViewBackGround, pieceViewSinglePieceView);
        pieceViewSinglePieceViewChildren.children.push_front(pieceViewSinglePieceViewBackGround);
        scene.registry.emplace<CanvasElement>(pieceViewSinglePieceViewBackGround, 0);
        scene.registry.emplace<Transform2D>(pieceViewSinglePieceViewBackGround, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(1.0f));
        scene.registry.emplace<Fill2D>(pieceViewSinglePieceViewBackGround, 
            glm::vec3(0.15f, 0.15f, 0.18f), 
            pieceViewSinglePieceViewWidth, 
            pieceViewSinglePieceViewHeight,
            30.0f
        );
        singlePieceViewComponent.background = pieceViewSinglePieceViewBackGround;

        auto pieceUIScene = scene.registry.create();
        scene.registry.emplace<Parent>(pieceUIScene, pieceViewSinglePieceView);
        pieceViewSinglePieceViewChildren.children.push_front(pieceUIScene);
        scene.registry.emplace<CanvasElement>(pieceUIScene, 1);
        scene.registry.emplace<Transform2D>(pieceUIScene, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec3(1.0f));
        auto& uiScene = scene.registry.emplace<UIScene>(pieceUIScene);
        uiScene.width = pieceViewSinglePieceViewWidth;
        uiScene.height = pieceViewSinglePieceViewHeight;
        singlePieceViewComponent.subscene = pieceUIScene;

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
        uiScene.scene.emplace<PuzzlePiece>(uiScenePiece);
        uiScene.scene.emplace<Transform>(uiScenePiece, glm::vec3(0.0f), glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(1.0f));
        auto& uiScenePieceChildren = uiScene.scene.emplace<Children>(uiScenePiece);

        for (auto block : pieceChildren.children) {
            auto uiScenePieceBlock = uiScene.scene.create();
            uiScene.scene.emplace<Parent>(uiScenePieceBlock, uiScenePiece);
            uiScene.scene.emplace<Model>(uiScenePieceBlock, scene.models["cube"].get());
            uiScene.scene.emplace<Shader>(uiScenePieceBlock, "shaders/phong/phong.vert", "shaders/phong/phong.frag");

            auto& transform = scene.registry.get<Transform>(block);
            auto& material = scene.registry.get<Material>(block);

            uiScene.scene.emplace<Material>(uiScenePieceBlock, material);
            uiScene.scene.emplace<Transform>(uiScenePieceBlock, transform);
        }

        addUIEntityToScrollView(scene.registry, pieceViewScrollView, pieceViewSinglePieceView);

        i++;
    }

    return pieceView;
}

entt::entity App::addPuzzleFromModel() {
    auto puzzle = scene.registry.create();

    scene.registry.emplace<Puzzle>(puzzle);
    scene.registry.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.registry.emplace<ExplodedView>(puzzle,0.0f);
    scene.registry.emplace<Children>(puzzle);

    PuzzleLoader loader = PuzzleLoader();
    auto result = loader.LoadSolution("resources/data/half_cube-4x4x4.txt");

    for (auto & item : result.pieces)
    {
        auto piece = addPiece(puzzle, item.origin, item.color);

        for (auto & block : item.blocks)
        {
            addBlock(piece, block);
        }
    }

    return puzzle;
}

entt::entity App::addPiece(entt::entity puzzle, glm::vec3 position, glm::vec3 color) {
    auto puzzleChildren = scene.registry.try_get<Children>(puzzle);
    if (puzzleChildren == nullptr) {
        throw std::runtime_error("App::addPiece: cannot add piece to entity without children");
    }

    auto piece = scene.registry.create();
    scene.registry.emplace<Parent>(piece, puzzle);
    scene.registry.emplace<Transform>(piece,
                             position,
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.registry.emplace<Children>(piece);

    puzzleChildren->children.push_front(piece);
    auto& pieceComponent = scene.registry.emplace<PuzzlePiece>(piece, position);
    pieceComponent.selected = false;
    pieceComponent.defaultColor = color;
    pieceComponent.selectionColor = color + glm::vec3(0.3f);

    return piece;
}


entt::entity App::addBlock(entt::entity piece, glm::vec3 position) {
    auto pieceComponent = scene.registry.try_get<PuzzlePiece>(piece);
    if ( pieceComponent == nullptr) {
        throw std::runtime_error("App::addBlock: cannot add block to entity that is not a piece");
    }
    auto pieceChildren = scene.registry.try_get<Children>(piece);
    if (pieceChildren == nullptr) {
        throw std::runtime_error("App::addBlock: cannot add block to entity without children");
    }

    auto block = scene.registry.create();

    scene.registry.emplace<Model>(block, scene.models["resources/models/wooden-cube/wooden-cube.obj"].get());
    scene.registry.emplace<Shader>(block, "shaders/phong/phong.vert", "shaders/phong/phong.frag");
    scene.registry.emplace<Material>(block,
        pieceComponent->defaultColor,
        pieceComponent->defaultColor * 0.15f + glm::vec3(0.1f, 0.1f, 0.12f),
        glm::vec3(0.0f),
        1.0f
    );
    scene.registry.emplace<Transform>(block,
        glm::vec3(position),
        glm::vec3(0.0f),
        glm::vec3(1.0f)
    );
    scene.registry.emplace<BoundingBox>(block,
        glm::vec3(0.0f),
        glm::vec3(0.5f),
        glm::vec3(0.5f),
        glm::vec3(0.5f) 
    );

    pieceChildren->children.push_front(block);
    scene.registry.emplace<Parent>(block, piece);

    return block;
}