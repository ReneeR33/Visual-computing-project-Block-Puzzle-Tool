#include "DebugWindow.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

DebugWindow::DebugWindow(GlfwWindow &window, Scene& scene) 
    : window(window)
    , scene(scene) 
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    ImGui_ImplOpenGL3_Init("#version 410");

    for (auto& item : scene.models) {
        sceneModelsVector.push_back({ .name = item.first, .model = &item.second});
    }
}

void DebugWindow::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Debug");
        ObjectInfo();
        CameraInfo();
        LightInfo();
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugWindow::ObjectInfo() {
    if (ImGui::CollapsingHeader("Objects")) {
        for (unsigned int i = 0; i < scene.objects.size(); i++) {
            if (ImGui::TreeNode(("cube " + std::to_string(i + 1)).c_str())) {
                Object& object = scene.objects[i];

                static std::vector<int>item_current_idx(scene.objects.size());

                if (item_current_idx.size() < scene.objects.size()) {
                    item_current_idx.push_back(0);
                }

                std::string combo_label = sceneModelsVector[item_current_idx[i]].name;

                if (sceneModelsVector[item_current_idx[i]].model != object.model) {
                    object.model = sceneModelsVector[item_current_idx[i]].model;
                }

                ImGui::Text("Model");
                if (ImGui::BeginCombo("model", combo_label.c_str())) {
                    for (unsigned int n = 0; n < scene.models.size(); n++) {
                        const bool is_selected = (item_current_idx[i] == n);

                        if (ImGui::Selectable(sceneModelsVector[n].name.c_str(), is_selected)) {
                            item_current_idx[i] = n;
                        }

                        if (is_selected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                ImGui::Text("Color");
                ImGui::ColorEdit3("color", &object.color.x);
                ImGui::ColorEdit3("ambient color", &object.ambientColor.x);
                ImGui::ColorEdit3("specular color", &object.specularColor.x);
                ImGui::SliderFloat("specular power", &object.specularPow, 1.0f, 5.0f);
                ImGui::Text("Transform");
                ImGui::SliderFloat3("position", &object.transform.position.x, -5.0f, 5.0f);
                ImGui::SliderFloat3("rotation", &object.transform.rotation.x, -360.0f, 360.0f);
                ImGui::SliderFloat3("scale", &object.transform.scale.x, 0.1f, 10.0f);
                ImGui::TreePop();
            }
        }
    }
}

void DebugWindow::CameraInfo() {
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat3("cam position", &scene.camera.position.x, -5.0f, 5.0f);
        ImGui::SliderFloat3("cam direction", &scene.camera.direction.x, -1.0f, 1.0f);
        ImGui::SliderFloat3("up", &scene.camera.up.x, -1.0f, 1.0f);
        ImGui::SliderFloat("near", &scene.camera.near, 0.0f, 200.0f);
        ImGui::SliderFloat("far", &scene.camera.far, 0.0f, 200.0f);
        ImGui::SliderFloat("fov", &scene.camera.fov, 0.0f, 360.0f);
    }
}

void DebugWindow::LightInfo() {
    if (ImGui::CollapsingHeader("Light")) {
        ImGui::SliderFloat3("light direction", &scene.dirLight.direction.x, -1.0f, 1.0f);
        ImGui::ColorEdit3("light ambient", &scene.dirLight.ambient.x);
        ImGui::ColorEdit3("light diffuse", &scene.dirLight.diffuse.x);
        ImGui::ColorEdit3("light specular", &scene.dirLight.specular.x);
    }
}
