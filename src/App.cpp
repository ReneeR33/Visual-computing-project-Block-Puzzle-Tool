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

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 950
#define WINDOW_NAME "puzzle tool"

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
}

void App::run() {
    Renderer renderer;
    PuzzleViewSystem::init(scene, window);

    //initTestScene();
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

void App::initTestScene() {
    auto background = scene.create();
    scene.emplace<Background>(background, glm::vec3(0.05f));

    auto dirLight = scene.create();
    scene.emplace<DirLight>(dirLight,
                            glm::vec3(0.0f, 0.14f, -1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f),
                            glm::vec3(1.0f)
    );

    auto camera = scene.create();
    scene.emplace<Camera>(camera,
                          glm::vec3(0.0f, 1.0f, 4.0f),
                          glm::vec3(0.0f, 0.0f, -1.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f),
                          0.1f, 100.0f, 80.0f
    );

    auto cube_1 = scene.create();
    scene.emplace<Model>(cube_1, primitives::cube);
    scene.emplace<Shader>(cube_1, "shaders/shader.vert", "shaders/shader.frag");
    scene.emplace<Material>(cube_1,
                            glm::vec3(1.0f, 0.0f, 0.0f),
                            glm::vec3(0.1f, 0.0f, 0.0f),
                            glm::vec3(0.0f),
                            1.0f
    );
    scene.emplace<Transform>(cube_1,
                             glm::vec3(1.5f, 2.0f, 0.0f),
                             glm::vec3(10.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f)
    );

    auto cube_2 = scene.create();
    scene.emplace<Model>(cube_2, primitives::cube);
    scene.emplace<Shader>(cube_2, "shaders/shader.vert", "shaders/shader.frag");
    scene.emplace<Material>(cube_2,
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            glm::vec3(0.0f, 0.1f, 0.0f),
                            glm::vec3(0.0f),
                            1.0f
    );
    scene.emplace<Transform>(cube_2,
                             glm::vec3(1.5f, 0.0f, -1.0f),
                             glm::vec3(10.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f)
    );

    auto cube_3 = scene.create();
    scene.emplace<Model>(cube_3, primitives::cube);
    scene.emplace<Shader>(cube_3, "shaders/shader.vert", "shaders/shader.frag");
    scene.emplace<Material>(cube_3,
                            glm::vec3(0.0f, 0.0f, 1.0f),
                            glm::vec3(0.0f, 0.0f, 0.1f),
                            glm::vec3(0.0f),
                            1.0f
    );
    scene.emplace<Transform>(cube_3,
                             glm::vec3(3.5f, 0.5f, -1.0f),
                             glm::vec3(10.f),
                             glm::vec3(1.0f, 1.0f, 1.0f)
    );
}

void App::initExplodedViewTestScene() {
    auto background = scene.create();
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

    for (int i_x = 0; i_x < 3; i_x++) {
        for (int i_y = 0; i_y < 3; i_y++) {
            for (int i_z = 0; i_z < 3; i_z++) {
                entt::entity cube = scene.create();

                glm::vec3 color = glm::vec3(float(i_x) * 0.333333f, float(i_y) * 0.333333f, float(i_z) * 0.333333f);
                glm::vec3 position = glm::vec3(-1.0f + float(i_x), -1.0f + float(i_y), -1.0f + float(i_z));

                scene.emplace<Model>(cube, primitives::cube);
                scene.emplace<Shader>(cube, "shaders/shader.vert", "shaders/shader.frag");
                scene.emplace<Material>(cube,
                                        color,
                                        glm::vec3(0.1f, 0.0f, 0.0f),
                                        glm::vec3(0.0f),
                                        1.0f
                );
                scene.emplace<PuzzlePiece>(cube, position);
                scene.emplace<Transform>(cube,
                                         glm::vec3(position),
                                         glm::vec3(0.0f),
                                         glm::vec3(1.0f, 1.0f, 1.0f)
                );

                puzzleChildren.children.push_front(cube);
                scene.emplace<Parent>(cube, puzzle);
            }
        }
    }
}
