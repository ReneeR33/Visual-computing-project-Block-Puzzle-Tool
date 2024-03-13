#include "Systems/Renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "Components/Parent.hpp"
#include "Components/Fill2D.hpp"
#include "Components/Transform2D.hpp"
#include "Components/UICanvas.hpp"
#include "Components/Children.hpp"
#include "Components/CanvasElement.hpp"
#include "Components/UIScene.hpp"

Mesh Renderer::fillMesh = {
    .vertices = {
            { .position = glm::vec3(-0.5f, -0.5f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f) },
            { .position = glm::vec3(-0.5f, 0.5f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f) },
            { .position = glm::vec3(0.5f, -0.5f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f) },
            { .position = glm::vec3(0.5f, 0.5f, 0.0f), .normal = glm::vec3(0.0f, 0.0f, 1.0f) },
            },
    .indices = {
            0, 1, 2,
            3, 1, 2
    }
};

Renderer::Renderer()
    : fillShader("shaders/fill/fill.vert", "shaders/fill/fill.frag") {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    //TODO: clearing mesh?
    load(fillMesh);
}

void Renderer::load(entt::registry &scene) {
    auto view = scene.view<Model>();
    for(auto [entity, model]: view.each()) {
        for (auto& mesh : model.meshes) {
            load(mesh);
        }
    }

    auto uiSceneView = scene.view<UIScene>();
    for (auto [entity, uiScene]: uiSceneView.each()) {
        auto view = uiScene.scene.view<Model>();
        for(auto [entity, model]: view.each()) {
            for (auto& mesh : model.meshes) {
                load(mesh);
            }
        }
    }
}

void Renderer::load(Mesh &mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    if (!mesh.indices.empty()) {
        glGenBuffers(1, &mesh.EBO);
    }

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    if (!mesh.indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glBindVertexArray(0);
}

void Renderer::render(entt::registry &scene) {
    auto& background = scene.get<Background>(scene.view<Background>().front());
    auto backgroundColor = background.color;
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    auto width = static_cast<float>(m_viewport[2]);
    auto height = static_cast<float>(m_viewport[3]);

    renderWorld(scene, width, height, glm::mat4(1.0f));
    renderUI(scene, width, height);
}

void Renderer::renderWorld(entt::registry &scene, float viewportWidth, float viewportHeight, glm::mat4 eTransform) {
    auto& camera = scene.get<Camera>(scene.view<Camera>().front());

    float aspect = viewportWidth / viewportHeight;

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), aspect, camera.near, camera.far);
    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.direction, glm::normalize(camera.up));

    auto& dirLight = scene.get<DirLight>(scene.view<DirLight>().front());

    auto entitiesView = scene.view<Model, Material, Shader, Transform>();
    for (auto& entity : entitiesView) {
        renderWorldObject(scene, entity, camera, dirLight, view, projection, eTransform);
    }
}

void Renderer::renderUI(entt::registry &scene, float viewportWidth, float viewportHeight) {
    //TODO: replace ortho with custom matrix
    glm::mat4 projection = glm::ortho(0.0f, viewportWidth, 0.0f, viewportHeight, -1.0f, 1.0f);

    auto canvasView = scene.view<UICanvas>();

    for (auto [canvasEntity, uiCanvas] : canvasView.each()) {
        renderUIElement(scene, canvasEntity, viewportWidth, viewportHeight, glm::mat4(1.0f), projection);
    }
}

void Renderer::renderUIElement(entt::registry &scene, const entt::entity &object, float viewportWidth, float viewportHeight, glm::mat4 model, glm::mat4 &projection) {
    glClear(GL_DEPTH_BUFFER_BIT);

    auto& transform = scene.get<Transform2D>(object);
    model = glm::translate(model, glm::vec3(transform.position, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, 1.0f));

    auto fill = scene.try_get<Fill2D>(object);
    if (fill != nullptr) {
        model = glm::scale(model, glm::vec3(fill->width, fill->height, 1.0f));

        fillShader.use();
        fillShader.setMat4("projection", projection);
        fillShader.setMat4("model", model);

        fillShader.setVec3("color", fill->color);

        draw(fillMesh);
    }

    auto uiScene = scene.try_get<UIScene>(object);
    if (uiScene != nullptr) {
        model = glm::scale(model, glm::vec3(uiScene->width / 2, uiScene->height / 2, 1.0f));

        glm::mat4 eTransform(1.0f);
        eTransform = glm::translate(eTransform, glm::vec3(-1, -1, 0));
        eTransform = glm::scale(eTransform, glm::vec3(2 / viewportWidth, 2 / viewportHeight, 1.0f));
        eTransform = eTransform * model;

        renderWorld(uiScene->scene, uiScene->width, uiScene->height, eTransform);
    }

    auto children = scene.try_get<Children>(object);
    if (children != nullptr) {
        children->children.sort([&] (entt::entity& e1, entt::entity& e2) {
            auto& canvasElement1 = scene.get<CanvasElement>(e1);
            auto& canvasElement2 = scene.get<CanvasElement>(e2);

            return canvasElement1.layer < canvasElement2.layer;
        });
        for (auto child : children->children) {
            renderUIElement(scene, child, viewportWidth, viewportHeight, model, projection);
        }
    }
}

void Renderer::renderWorldObject(entt::registry& scene, const entt::entity &object, Camera camera, DirLight dirlight, glm::mat4 &view, glm::mat4 &projection, glm::mat4& eTransform) {
    auto& shader = scene.get<Shader>(object);
    auto& material = scene.get<Material>(object);
    auto& model = scene.get<Model>(object);

    std::list<glm::mat4> modelMatrices;

    auto cEntity = &object;
    while (cEntity != nullptr) {
        auto transform = scene.try_get<Transform>(*cEntity);

        if (transform != nullptr) {
            auto modelm = glm::mat4(1.0f);
            modelm = glm::translate(modelm, transform->position);
            float rotXRadians = (transform->rotation.x / 180.0f) * glm::pi<float>();
            float rotYRadians = (transform->rotation.y / 180.0f) * glm::pi<float>();
            float rotZRadians = (transform->rotation.z / 180.0f) * glm::pi<float>();
            modelm = glm::rotate(modelm, rotXRadians, glm::vec3(1, 0, 0));
            modelm = glm::rotate(modelm, rotYRadians, glm::vec3(0, 1, 0));
            modelm = glm::rotate(modelm, rotZRadians, glm::vec3(0, 0, 1));
            modelm = glm::scale(modelm, transform->scale);

            modelMatrices.push_back(modelm);
        }

        auto parentComponent = scene.try_get<Parent>(*cEntity);
        if (parentComponent != nullptr) {
            cEntity = &parentComponent->parent;
        } else {
            cEntity = nullptr;
        }
    }

    auto modelm = glm::mat4(1.0f);
    for (const auto & modelMatrix : modelMatrices) {
        modelm = modelMatrix * modelm;
    }

    shader.use();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", modelm);
    shader.setMat4("eTransform", eTransform);

    shader.setVec3("color", material.color);
    shader.setVec3("ambient", material.ambientColor);
    shader.setVec3("specular", material.specularColor);
    shader.setFloat("specularPow", material.specularPow);

    shader.setVec3("viewPos", camera.position);

    shader.setVec3("dirLight.direction", dirlight.direction);
    shader.setVec3("dirLight.diffuse", dirlight.diffuse);
    shader.setVec3("dirLight.ambient", dirlight.ambient);
    shader.setVec3("dirLight.specular", dirlight.specular);

    for(auto& mesh : model.meshes) {
        draw(mesh);
    }
}

void Renderer::draw(Mesh &mesh) {
    glBindVertexArray(mesh.VAO);
    
    if (!mesh.indices.empty()) {
        glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.vertices.size());
    }

    glBindVertexArray(0);
}
