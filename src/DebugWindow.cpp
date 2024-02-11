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
}

void DebugWindow::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("Settings");

        if (ImGui::CollapsingHeader("Objects"))
        {
            for (unsigned int i = 0; i < scene.objects.size(); i++)
            {
                if (ImGui::TreeNode(("cube " + std::to_string(i + 1)).c_str()))
                {
                    Object& object = scene.objects[i];
                    
                    static std::vector<int>item_current_idx(scene.objects.size());

                    if (item_current_idx.size() < scene.objects.size())
                    {
                        item_current_idx.push_back(0);
                    }

                    std::string combo_label = scene.models[item_current_idx[i]].name;

                    if (combo_label != object.model->name)
                    {
                        object.model = &scene.models[item_current_idx[i]];
                    }

                    ImGui::Text("Model");
                    if (ImGui::BeginCombo("model", combo_label.c_str()))
                    {
                        for (unsigned int n = 0; n < scene.models.size(); n++)
                        {
                            const bool is_selected = (item_current_idx[i] == n);

                            if (ImGui::Selectable(scene.models[n].name.c_str(), is_selected))
                            {
                                item_current_idx[i] = n;
                            }

                            if (is_selected)
                            {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Text("Color");
                    ImGui::ColorEdit3("color", &object.color.x);
                    ImGui::Text("Transform");
                    ImGui::SliderFloat3("position", &object.transform.position.x, -5.0f, 5.0f);
                    ImGui::SliderFloat3("rotation", &object.transform.rotation.x, -360.0f, 360.0f);
                    ImGui::TreePop();
                }
            }
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}