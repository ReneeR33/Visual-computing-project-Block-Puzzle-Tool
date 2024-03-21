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
#include "Components/Solution.hpp"
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

    {
        ImGui::Begin("Debug");

        auto& background = scene.get<Background>(scene.view<Background>().front());
        ImGui::ColorEdit3("background", &background.color.x);

        auto puzzleView = scene.view<Puzzle>();
        if (!puzzleView.empty()) {
            auto& puzzle = scene.get<Puzzle>(puzzleView.front());
            ImGui::Checkbox("disable puzzle mouse rotation", &puzzle.disableMouseRotation);
        }

        ObjectInfo(scene);
        CameraInfo(scene);
        LightInfo(scene);
        ExplodedViewInfo(scene);
        SolutionInfo(scene);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindow::ObjectInfo(entt::registry& scene) {
    if (ImGui::CollapsingHeader("Objects")) {
        auto puzzleView = scene.view<Puzzle>();
        if (puzzleView.empty()) {
            return;
        }

        ImGui::Text("Puzzle");
        auto puzzle = puzzleView.front();
        auto& puzzleTransform = scene.get<Transform>(puzzle);

        ImGui::SliderFloat3("puzzle position", &puzzleTransform.position.x, -5.0f, 5.0f);
        ImGui::SliderFloat3("puzzle rotation", &puzzleTransform.rotation.x, -360.0f, 360.0f);
        ImGui::SliderFloat3("puzzle scale", &puzzleTransform.scale.x, 0.1f, 10.0f);

        auto puzzleChildren = scene.try_get<Children>(puzzle);
        if (puzzleChildren == nullptr) {
            return;
        }

        if (ImGui::TreeNode("Pieces")) {
            int i = 0;
            for (auto piece : puzzleChildren->children) {
                auto& pieceTransform = scene.get<Transform>(piece);
                if (ImGui::TreeNode(("piece " + std::to_string(i + 1)).c_str())) {
                    ImGui::Text("Transform");
                    ImGui::SliderFloat3("position", &pieceTransform.position.x, -5.0f, 5.0f);
                    ImGui::SliderFloat3("rotation", &pieceTransform.rotation.x, -360.0f, 360.0f);
                    ImGui::SliderFloat3("scale", &pieceTransform.scale.x, 0.1f, 10.0f);

                    auto pieceChildren = scene.try_get<Children>(piece);
                    if (pieceChildren != nullptr) {
                        int j = 0;
                        for (auto block : pieceChildren->children) {
                            if (ImGui::TreeNode(("block " + std::to_string(j + 1)).c_str())) {
                                auto& blockTransform = scene.get<Transform>(block);
                                auto& material = scene.get<Material>(block);

                                ImGui::Text("Transform");
                                ImGui::SliderFloat3("position", &blockTransform.position.x, -5.0f, 5.0f);
                                ImGui::SliderFloat3("rotation", &blockTransform.rotation.x, -360.0f, 360.0f);
                                ImGui::SliderFloat3("scale", &blockTransform.scale.x, 0.1f, 10.0f);

                                ImGui::Text("Color");
                                ImGui::ColorEdit3("color", &material.color.x);
                                ImGui::ColorEdit3("ambient color", &material.ambientColor.x);
                                ImGui::ColorEdit3("specular color", &material.specularColor.x);
                                ImGui::SliderFloat("specular power", &material.specularPow, 1.0f, 5.0f);

                                ImGui::TreePop();
                            }
                            j++;
                        }
                    }

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

void DebugWindow::SolutionInfo(entt::registry &scene) {
    auto view = scene.view<Puzzle>();
    if (view.empty()) {
        return;
    }

    int total_range = 0;
    auto puzzle = view.front();
    auto puzzleChildren = scene.try_get<Children>(puzzle);
    if (puzzleChildren == nullptr) {
        return;
    }
    // std::cout << "came here 1" << std::endl;
    for (auto piece : puzzleChildren->children) {
        auto& solution = scene.get<Solution>(piece);
        total_range += solution.Solution.size()-1;
    }

    if (ImGui::CollapsingHeader("solution steps")) {

        auto puzzleView = scene.view<Puzzle>();
        if (!puzzleView.empty()) {
            auto& puzzle = scene.get<Puzzle>(puzzleView.front());
            // ImGui::Checkbox("disable puzzle mouse rotation", &puzzle.disableMouseRotation);
            ImGui::SliderInt("solution", &puzzle.solutionStep, 0, total_range);
        }
    }
}
