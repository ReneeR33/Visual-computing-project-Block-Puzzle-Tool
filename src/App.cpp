#include "App.hpp"

#include <map>
#include "Shader.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"
#include "DebugWindow.hpp"
#include "primitives.hpp"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
#define WINDOW_NAME "puzzle tool"

App::App() : window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME) {
    
}

void App::run() {
    Renderer renderer;
    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    Scene scene;
    scene.backgroundColor = glm::vec3(0.05f);

    scene.dirLight = {
        .direction = glm::vec3(0.0f, 0.14f, -1.0f),
        .diffuse = glm::vec3(1.0f),
        .ambient = glm::vec3(1.0f),
        .specular = glm::vec3(1.0f)
    };
    scene.camera = {
        .position = glm::vec3(0.0f, 1.0f, 4.0f),
        .direction = glm::vec3(0.0f, 0.0f, -1.0f),
        .up = glm::vec3(0.0f, 1.0f, 0.0f),
        .near = 0.1f,
        .far = 100.0f,
        .fov = 80.0f
    }; 

    scene.models["cube"] = primitives::cube;
    scene.objects.push_back({
        .model = &scene.models["cube"],
        .shader = &shader,
        .color = glm::vec3(1.0f, 0.0f, 0.0f),
        .ambientColor = glm::vec3(0.1f, 0.0f, 0.0f),
        .specularColor = glm::vec3(0.0f),
        .specularPow = 1.0f,
        .transform = {
            .position = glm::vec3(1.5f, 2.0f, 0.0f),
            .rotation = glm::vec3(10.0f),
            .scale = glm::vec3(1.0f, 1.0f, 1.0f),
        }
    });
    scene.objects.push_back({
        .model = &scene.models["cube"],
        .shader = &shader,
        .color = glm::vec3(0.0f, 1.0f, 0.0f),
        .ambientColor = glm::vec3(0.0f, 0.1f, 0.0f),
        .specularColor = glm::vec3(0.0f),
        .specularPow = 1.0f,
        .transform = {
            .position = glm::vec3(1.5f, 0.0f, -1.0f),
            .rotation = glm::vec3(10.0f),
            .scale = glm::vec3(1.0f, 1.0f, 1.0f),
        }
    });
    scene.objects.push_back({
            .model = &scene.models["cube"],
            .shader = &shader,
            .color = glm::vec3(0.0f, 0.0f, 1.0f),
            .ambientColor = glm::vec3(0.0f, 0.0f, 0.1f),
            .specularColor = glm::vec3(0.0f),
            .specularPow = 1.0f,
            .transform = {
                    .position = glm::vec3(3.5f, 0.5f, -1.0f),
                    .rotation = glm::vec3(10.f),
                    .scale = glm::vec3(1.0f, 1.0f, 1.0f),
            }
    });

    renderer.load(scene);
    DebugWindow debugWindow(window, scene);

    while (!window.windowShouldClose()) {
        renderer.newFrame();
        renderer.render(scene);

        debugWindow.render();

        window.update();
    }
}