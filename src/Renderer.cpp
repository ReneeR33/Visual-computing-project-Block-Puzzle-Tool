#include "Systems/Renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include "Components/Parent.hpp"


Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::load(entt::registry &scene) {
    auto view = scene.view<Model>();
    for(auto [entity, model]: view.each()) {
        for (auto& mesh : model.meshes) {
            load(mesh);
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

    auto& camera = scene.get<Camera>(scene.view<Camera>().front());

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    auto width = static_cast<float>(m_viewport[2]);
    auto height = static_cast<float>(m_viewport[3]);
    float aspect = width / height;

    float fovRadians = (camera.fov / 180.0f) * glm::pi<float>();
    glm::mat4 projection = glm::perspective(fovRadians, aspect, camera.near, camera.far);
    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.direction, glm::normalize(camera.up));

    auto& dirLight = scene.get<DirLight>(scene.view<DirLight>().front());

    auto renderableEntitiesView = scene.view<Model, Material, Shader, Transform>();
    for (auto& entity : renderableEntitiesView) {
        render(scene, entity, camera, dirLight, view, projection);
    }
}

void Renderer::render(entt::registry& scene, const entt::entity &object, Camera camera, DirLight dirlight, glm::mat4 &view, glm::mat4 &projection) {
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

    shader.setVec4("color", material.color);
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