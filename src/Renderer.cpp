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

#define SHADOW_MAP_HEIGHT 2024
#define SHADOW_MAP_WIDTH 2024
#define SHADOW_MAP_FRUSTUM_LEFT -10.0f
#define SHADOW_MAP_FRUSTUM_RIGHT 10.0f
#define SHADOW_MAP_FRUSTUM_BOTTOM -10.0f
#define SHADOW_MAP_FRUSTUM_TOP 10.0f

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
    : fillShader("shaders/fill/fill.vert", "shaders/fill/fill.frag")
    , shadowMapShader("shaders/shadowmap/shadowmap.vert", "shaders/shadowmap/shadowmap.frag") {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    //TODO: clearing mesh?
    load(fillMesh);

    // For depth mapping
    glGenFramebuffers(1, &depthMapFrameBuffer);

    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
        SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 
        0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    auto lightProjection = glm::ortho(
        SHADOW_MAP_FRUSTUM_LEFT, SHADOW_MAP_FRUSTUM_RIGHT,
        SHADOW_MAP_FRUSTUM_BOTTOM, SHADOW_MAP_FRUSTUM_TOP,
        0.0f, 40.0f
    );
    auto lightView = glm::lookAt(
        -(10.0f * dirLight.direction),
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    auto lightSpaceMatrix = lightProjection * lightView;

    renderDepthMap(scene, lightSpaceMatrix);

    auto entitiesView = scene.view<Model, Material, Shader, Transform>();
    for (auto& entity : entitiesView) {
        renderWorldObject(scene, entity, camera, dirLight, view, projection, lightSpaceMatrix, eTransform);
    }
}

void Renderer::renderWorldObject(
        entt::registry& scene, const entt::entity &object, 
        Camera camera, DirLight dirlight, 
        glm::mat4 &view, glm::mat4 &projection, glm::mat4 &lightSpace, glm::mat4& eTransform
    ) {
    auto& shader = scene.get<Shader>(object);
    auto& material = scene.get<Material>(object);
    auto& model = scene.get<Model>(object);

    auto modelMatrix = createModelMatrix(scene, object);

    shader.use();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", modelMatrix);
    shader.setMat4("eTransform", eTransform);
    shader.setMat4("lightSpaceMatrix", lightSpace);

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

void Renderer::renderDepthMap(entt::registry& scene, glm::mat4& lightSpaceMatrix) {
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto entitiesView = scene.view<Model, Material, Transform>();
    for (auto [entity, model, material, transform] : entitiesView.each()) {

        auto modelMatrix = createModelMatrix(scene, entity);

        shadowMapShader.use();
        shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shadowMapShader.setMat4("model", modelMatrix);

        for (auto& mesh : model.meshes) {
            draw(mesh);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_viewport[2], m_viewport[3]);
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

glm::mat4 Renderer::createModelMatrix(entt::registry& scene, entt::entity worldObject) {
    auto cEntity = &worldObject;
    auto modelm = glm::mat4(1.0f);
    while (cEntity != nullptr) {
        auto transform = scene.try_get<Transform>(*cEntity);

        if (transform != nullptr) {
            auto localmodelm = glm::mat4(1.0f);
            localmodelm = glm::translate(localmodelm, transform->position);
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.x), glm::vec3(1, 0, 0));
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.y), glm::vec3(0, 1, 0));
            localmodelm = glm::rotate(localmodelm, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1));
            localmodelm = glm::scale(localmodelm, transform->scale);

            modelm = localmodelm * modelm;
        }

        auto parentComponent = scene.try_get<Parent>(*cEntity);
        if (parentComponent != nullptr) {
            cEntity = &parentComponent->parent;
        } else {
            cEntity = nullptr;
        }
    }

    return modelm;
}