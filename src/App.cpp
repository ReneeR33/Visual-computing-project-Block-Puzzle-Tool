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
#include "Components/Button.hpp"
#include "InputSystem.hpp"
#include "primitives.hpp"
#include "PuzzleLoader.hpp"
#include "UI.hpp"
#include "entity.hpp"

#include "tinyfiledialogs.h"
#include "tinyfiledialogs.c"
#include "SolutionFinder.hpp"

#include "ModelLoader.hpp"


#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

#define PIECE_VIEW_WIDTH WINDOW_WIDTH / 5

//#define LOAD_TEST_PUZZLE

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
    ModelLoader modelLoader;
    modelLoader.loadModel(scene, "resources/models/wooden-cube/wooden-cube.obj");
    scene.models["cube"] = std::make_unique<ModelData>(primitives::cube_data);
    scene.textures["background"] = std::make_unique<TextureData>(TextureData{
        .path = "resources/textures/background.jpg"
    });
}

void App::run() {
    InputSystem::init(window);

    Renderer renderer;
    UISystem uiSystem(scene.registry);
    PuzzleViewSystem puzzleViewSystem(scene);
    PieceViewSystem pieceViewSystem(scene);

    resetScene();

    auto uiCanvas = scene.registry.create();
    scene.registry.emplace<UICanvas>(uiCanvas);
    scene.registry.emplace<Transform2D>(uiCanvas, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    scene.registry.emplace<Children>(uiCanvas);

    addUI(uiCanvas);

    renderer.load(scene);
    DebugWindow debugWindow(window);

    while (!window.windowShouldClose()) {
        InputSystem::update();

        uiSystem.update();
        puzzleViewSystem.update();
        pieceViewSystem.update();

        renderer.render(scene.registry);
        debugWindow.render(scene.registry);

        window.update();
    }
}

void App::resetScene() {
    scene.registry.clear();
    auto background = scene.registry.create();
    // TODO: change background
    scene.registry.emplace<Background>(background, glm::vec3(0.6f, 0.6f, 0.62f));

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
}

void App::initScene(entt::entity puzzle) {
    auto uiCanvas = scene.registry.create();
    scene.registry.emplace<UICanvas>(uiCanvas);
    scene.registry.emplace<Transform2D>(uiCanvas, glm::vec2(0.0f), 0.0f, glm::vec2(1.0f));
    auto& canvasChildren = scene.registry.emplace<Children>(uiCanvas);

    auto pieceViewEntityView = scene.registry.view<PiecesView>();
    if (!pieceViewEntityView.empty()) {
        auto pieceViewEntity = pieceViewEntityView.front();
        scene.registry.destroy(pieceViewEntity);
    }

    addPieceView(uiCanvas, puzzle);

    // UI -------------------------
    // TODO: color tuning
    // TODO: fill2d + transform bug
    
    addUI(uiCanvas);

    // ----------------------------
}

entt::entity App::addTestPuzzle() {
    auto puzzle = scene.registry.create();
    Solution solution;

    scene.registry.emplace<Puzzle>(puzzle);
    scene.registry.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.registry.emplace<ExplodedView>(puzzle,0.0f);
    scene.registry.emplace<Children>(puzzle);

    auto color = glm::vec3(0.6f,0.0f,0.0f);
    auto piece_1 = addPiece(puzzle, glm::vec3(-1.0f, 0.0f, -1.0f), color, solution);
    addBlock(piece_1, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_1, glm::vec3(1.0f,0.0f,0.0f));
    addBlock(piece_1, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_1, glm::vec3(0.0f,0.0f,1.0f));
    addBlock(piece_1, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.3f, 0.0f, 0.3f);
    auto piece_2 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 0.0f), color, solution);
    addBlock(piece_2, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_2, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_2, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.3f, 0.0f, 0.6f);
    auto piece_3 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, -1.0f), color, solution);
    addBlock(piece_3, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_3, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_3, glm::vec3(-1.0f,1.0f,0.0f));
    addBlock(piece_3, glm::vec3(-1.0f,-1.0f,0.0f));
    addBlock(piece_3, glm::vec3(0.0f,-1.0f,0.0f));

    color = glm::vec3(0.6f, 0.6f, 0.0f);
    auto piece_4 = addPiece(puzzle, glm::vec3(0.0f, 0.0f, 1.0f), color, solution);
    addBlock(piece_4, glm::vec3(0.0f,0.0f,0.0f));
    addBlock(piece_4, glm::vec3(0.0f,1.0f,0.0f));
    addBlock(piece_4, glm::vec3(0.0f,-1.0f,0.0f));
    addBlock(piece_4, glm::vec3(-1.0f,0.0f,0.0f));
    addBlock(piece_4, glm::vec3(1.0f,0.0f,0.0f));

    color = glm::vec3(0.3f, 0.3f, 0.6f);
    auto piece_5 = addPiece(puzzle, glm::vec3(-1.0f, 1.0f, 0.5f), color, solution);
    addBlock(piece_5, glm::vec3(0.0f,0.0f,0.5f));
    addBlock(piece_5, glm::vec3(0.0f,0.0f,-0.5f));

    color = glm::vec3(0.3f, 0.6f, 0.0f);
    auto piece_6 = addPiece(puzzle, glm::vec3(-1.0f, -1.0f, 0.5f), color, solution);
    addBlock(piece_6, glm::vec3(0.0f,0.0f,0.5f));
    addBlock(piece_6, glm::vec3(0.0f,0.0f,-0.5f));

    color = glm::vec3(0.3f, 0.6f, 0.3f);
    auto piece_7 = addPiece(puzzle, glm::vec3(1.0f, 0.0f, 0.0f), color, solution);
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
    scene.registry.emplace<Fill2D>(pieceViewBackground, glm::vec3(0.3f, 0.3f, 0.34f), float(PIECE_VIEW_WIDTH), float(WINDOW_HEIGHT));
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
            glm::vec3(0.5f, 0.5f, 0.56f),
            glm::vec3(0.6f, 0.6f, 0.66f),
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
            glm::vec3(0.1f, 0.1f, 0.12f), 
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
            uiScene.scene.emplace<Model>(uiScenePieceBlock, scene.models["resources/models/wooden-cube/wooden-cube.obj"].get());
            uiScene.scene.emplace<Shader>(uiScenePieceBlock, "shaders/phong/phong.vert", "shaders/phong/phong.frag");

            auto& transform = scene.registry.get<Transform>(block);
            auto& material = scene.registry.get<Material>(block);

            auto& uiSceneMaterial = uiScene.scene.emplace<Material>(uiScenePieceBlock, material);
            uiSceneMaterial.transparency = 1.0f;

            uiScene.scene.emplace<Transform>(uiScenePieceBlock, transform);
        }

        addUIEntityToScrollView(scene.registry, pieceViewScrollView, pieceViewSinglePieceView);

        i++;
    }

    return pieceView;
}

entt::entity App::addUI(entt::entity canvas) {
    auto& canvasChildren = scene.registry.get<Children>(canvas);

    auto UIEntity = scene.registry.create();
    scene.registry.emplace<Parent>(UIEntity, canvas);
    canvasChildren.children.push_back(UIEntity);
    scene.registry.emplace<CanvasElement>(UIEntity, 1, 300.0f, -300.0f, -200.0f, 200.0f);
    scene.registry.emplace<Transform2D>(UIEntity, glm::vec2(210.0f, 540.0f), 0.0f, glm::vec2(1.0f));
    auto& UIChildren = scene.registry.emplace<Children>(UIEntity);

    auto UIBackground = scene.registry.create();
    scene.registry.emplace<Parent>(UIBackground, UIEntity);
    UIChildren.children.push_front(UIBackground);
    scene.registry.emplace<CanvasElement>(UIBackground, 0);
    scene.registry.emplace<Transform2D>(UIBackground, glm::vec2(0.0f), 0.0f, glm::vec3(1.0f));
    scene.registry.emplace<Fill2D>(UIBackground, glm::vec3(0.3f, 0.3f, 0.34f), 400.0f, 600.0f, 50.0f);

    auto button = addButton(scene.registry, 1, -50.0f, 50.0f, -100.0f, 100.0f, glm::vec2(0.0f, 80.0f));
    scene.registry.emplace<Parent>(button, UIEntity);
    UIChildren.children.push_back(button);

    auto& buttonComponent = scene.registry.get<Button>(button);
    buttonComponent.buttonPressEvent.connect<&App::onLoadPuzzleButtonPressed>(*this);

    return UIEntity;
}

entt::entity App::addPuzzleFromModel(std::string path) {
    auto puzzle = scene.registry.create();

    scene.registry.emplace<Puzzle>(puzzle);
    scene.registry.emplace<Transform>(puzzle,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f),
                             glm::vec3(1.0f));
    scene.registry.emplace<ExplodedView>(puzzle,0.0f);
    scene.registry.emplace<Children>(puzzle);

    auto result = loader.LoadSolution(path);
    glm::vec3 size = loader.LoadSize(path);

    SolutionFinder finder = SolutionFinder(size);
    std::vector<Solution> solutions = finder.GetSolution(result); 

    for(int i = result.pieces.size() - 1; i >= 0 ; i--)
    {
        auto item = result.pieces[i];
        Solution solution = solutions[i];
        auto piece = addPiece(puzzle, item.origin, item.color, solution);

        for (auto & block : item.blocks)
        {
            addBlock(piece, block);
        }
    }

    return puzzle;
}

entt::entity App::addPiece(entt::entity puzzle, glm::vec3 position, glm::vec3 color, Solution solution) {
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
    pieceComponent.otherPieceSelectedTransparency = 0.1f;

    scene.registry.emplace<Solution>(piece, solution);

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
        glm::vec3(0.12f),
        4.0f,
        1.0f
    );
    scene.registry.emplace<Transform>(block,
        glm::vec3(position),
        glm::vec3(0.0f),
        glm::vec3(0.9999f)
    );
    scene.registry.emplace<BoundingBox>(block,
        0.5f,
        0.5f,
        0.5f
    );

    pieceChildren->children.push_front(block);
    scene.registry.emplace<Parent>(block, piece);

    return block;
}

void App::onLoadPuzzleButtonPressed() {
    #ifndef LOAD_TEST_PUZZLE
    char const * filter[2] = { "*.txt", "*.text" };
    char const * filename = tinyfd_openFileDialog(
        "Solution file", "./resources/solutions", 2, filter, "text files", 1);

    if (filename)
    {
        resetScene();
        std::string path(filename);
        entt::entity puzzle = addPuzzleFromModel(path);
        initScene(puzzle);
    }
    #else
        resetScene();
        entt::entity puzzle = addTestPuzzle();
        initScene(renderer, puzzle);
    #endif
}