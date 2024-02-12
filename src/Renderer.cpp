#include "Renderer.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void Renderer::newFrame() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::load(Scene &scene) {
    for (auto& item : scene.models) {
        auto& model = item.second;
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

void Renderer::render(Scene &scene) {
    Camera& camera = scene.camera;

    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    auto width = static_cast<float>(m_viewport[2]);
    auto height = static_cast<float>(m_viewport[3]);
    float aspect = width / height;

    glm::mat4 projection = glm::perspective(camera.fov, aspect, camera.near, camera.far);
    glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.direction, camera.up);

    for (auto& object : scene.objects) {
        render(scene, object, view, projection);
    }
}

void Renderer::render(Scene &scene, Object &object, glm::mat4 &view, glm::mat4 &projection) {
    auto& transform = object.transform;
    auto& shader = object.shader;

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, transform.rotation.x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0, 0, 1));
    model = glm::scale(model, transform.scale);

    shader->use();

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);

    shader->setVec3("color", object.color);
    shader->setVec3("ambient", object.ambientColor);
    shader->setVec3("specular", object.specularColor);
    shader->setFloat("specularPow", object.specularPow);

    shader->setVec3("viewPos", scene.camera.position);

    shader->setVec3("dirLight.direction", scene.dirLight.direction);
    shader->setVec3("dirLight.diffuse", scene.dirLight.diffuse);
    shader->setVec3("dirLight.ambient", scene.dirLight.ambient);

    for(auto& mesh : object.model->meshes) {
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