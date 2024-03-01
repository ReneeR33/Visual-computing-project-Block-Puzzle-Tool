#include "DebugWindow.hpp"

#include "Components/Background.hpp"
#include "Components/Material.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Components/Model.hpp"
#include "Components/Transform.hpp"
#include "Components/Camera.hpp"
#include "Components/DirLight.hpp"
#include "Components/ExplodedView.hpp"
#include "Components/PuzzlePiece.hpp"
#include "Components/Puzzle.hpp"
#include "Components/Children.hpp"

DebugWindow::DebugWindow(GlfwWindow &window) 
    : window(window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void DebugWindow::render(entt::registry& scene) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto& background = scene.get<Background>(scene.view<Background>().front());

    {
        ImGui::Begin("Debug");
        ImGui::ColorEdit3("background", &background.color.x);
        ObjectInfo(scene);
        CameraInfo(scene);
        LightInfo(scene);
        ExplodedViewInfo(scene);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindow::ObjectInfo(entt::registry& scene) {
    if (ImGui::CollapsingHeader("Objects")) {
        auto puzzleView = scene.view<Puzzle>();
        if (!puzzleView.empty()) {
            ImGui::Text("Puzzle");
            auto puzzle = puzzleView.front();
            auto& puzzleTransform = scene.get<Transform>(puzzle);

            ImGui::SliderFloat3("puzzle position", &puzzleTransform.position.x, -5.0f, 5.0f);
            ImGui::SliderFloat3("puzzle rotation", &puzzleTransform.rotation.x, -360.0f, 360.0f);
            ImGui::SliderFloat3("puzzle scale", &puzzleTransform.scale.x, 0.1f, 10.0f);
        }

        if (ImGui::TreeNode("Pieces")) {
            auto view = scene.view<PuzzlePiece>();

            int i = 0;
            for (auto [entity, piece] : view.each()) {
                auto& transform = scene.get<Transform>(entity);
                auto& material = scene.get<Material>(entity);
                if (ImGui::TreeNode(("cube " + std::to_string(i + 1)).c_str())) {
                    ImGui::Text("Color");
                    ImGui::ColorEdit3("color", &material.color.x);
                    ImGui::ColorEdit3("ambient color", &material.ambientColor.x);
                    ImGui::ColorEdit3("specular color", &material.specularColor.x);
                    ImGui::SliderFloat("specular power", &material.specularPow, 1.0f, 5.0f);
                    ImGui::Text("Transform");
                    ImGui::SliderFloat3("position", &transform.position.x, -5.0f, 5.0f);
                    ImGui::SliderFloat3("rotation", &transform.rotation.x, -360.0f, 360.0f);
                    ImGui::SliderFloat3("scale", &transform.scale.x, 0.1f, 10.0f);
                    ImGui::TreePop();
                }
                i++;
            }
            ImGui::TreePop();
        }
    }
}

void DebugWindow::CameraInfo(entt::registry& scene) {
    auto& camera = scene.get<Camera>(scene.view<Camera>().front());

    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat3("cam position", &camera.position.x, -5.0f, 5.0f);
        ImGui::SliderFloat3("cam direction", &camera.direction.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("up", &camera.up.x, -1.0f, 1.0f);
        ImGui::SliderFloat("near", &camera.near, 0.0f, 200.0f);
        ImGui::SliderFloat("far", &camera.far, 0.0f, 200.0f);
        ImGui::SliderFloat("fov", &camera.fov, 0.0f, 180.0f);
    }
}

void DebugWindow::LightInfo(entt::registry& scene) {
    auto& dirLight = scene.get<DirLight>(scene.view<DirLight>().front());

    if (ImGui::CollapsingHeader("Light")) {
        ImGui::SliderFloat3("light direction", &dirLight.direction.x, -1.0f, 1.0f);
        ImGui::ColorEdit3("light ambient", &dirLight.ambient.x);
        ImGui::ColorEdit3("light diffuse", &dirLight.diffuse.x);
        ImGui::ColorEdit3("light specular", &dirLight.specular.x);
    }
}

void DebugWindow::ExplodedViewInfo(entt::registry &scene) {
    if (ImGui::CollapsingHeader("Exploded View")) {
        auto view = scene.view<ExplodedView>();
        if (view.empty()) {
            return;
        }
        auto& exploded_view = scene.get<ExplodedView>(view.front());
        ImGui::SliderFloat("Exploded View Offset", &exploded_view.offset, 0.0f, 1.2f);
    }
}
