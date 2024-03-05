#include "App.hpp"

#include "Components/Background.hpp"
#include "Components/Shader.hpp"
#include "Systems/Renderer.hpp"
#include "DebugWindow.hpp"
#include "Components/Material.hpp"
#include "primitives.hpp"
#include "ModelLoader.hpp"
#include <iostream>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define WINDOW_NAME "puzzle tool"

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
}

void App::run() {
    Renderer renderer;
    ModelLoader loader = ModelLoader(scene);

    auto background = scene.create();
    scene.emplace<Background>(background, glm::vec3(0.05f));

    loader.LoadSolution("resources/data/puzzle_test.json");

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

    renderer.load(scene);
    DebugWindow debugWindow(window);

    while (!window.windowShouldClose()) {
        renderer.render(scene);
        debugWindow.render(scene);

        window.update();
    }
}