#include "App.hpp"

#include <map>
#include <entt/entt.hpp>
#include "Background.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "DebugWindow.hpp"
#include "Material.hpp"
#include "primitives.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define WINDOW_NAME "puzzle tool"

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
}

void App::run() {
    Renderer renderer;
    Shader shader("shaders/shader.vert", "shaders/shader.frag");

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

    renderer.load(scene);
    DebugWindow debugWindow(window);

    while (!window.windowShouldClose()) {
        renderer.render(scene);
        debugWindow.render(scene);

        window.update();
    }
}