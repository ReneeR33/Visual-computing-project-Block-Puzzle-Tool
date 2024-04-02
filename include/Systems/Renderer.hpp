#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include "glm/glm.hpp"
#include "entt/entt.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "TextureData.hpp"
#include "Components/Background.hpp"
#include "Components/Model.hpp"
#include "Components/Transform.hpp"
#include "Components/Material.hpp"
#include "Components/Camera.hpp"
#include "Components/DirLight.hpp"
#include "Components/Shader.hpp"

class Renderer {
public:
    Renderer();

    void load(Scene& scene);
    void render(entt::registry& scene);

private:
    Shader fillShader;
    Shader shadowMapShader;
    Shader screenShader;
    static Mesh fillMesh;
    static Mesh screenMesh;

    // shadow mapping
    unsigned int depthMapFrameBuffer;
    unsigned int depthMapTexture;

    // OIP transparency
    unsigned int opaqueFrameBuffer;
    unsigned int opaqueTexture;
    // TODO: this does not need to be a texture
    unsigned int opaqueDepthTexture;

    void load(Mesh& mesh);
    void load(TextureData& textureData);

    void prepareRenderFramebuffers();

    void renderWorld(entt::registry& scene, float viewportWidth, float viewportHeight, glm::mat4 eTransform);
    void renderBackBufferToScreen();
    void renderWorldObject(
        entt::registry& scene, const entt::entity& object, 
        Camera camera, DirLight dirlight, 
        glm::mat4& view, glm::mat4& projection, glm::mat4& lightSpace, glm::mat4& eTransform
    );
    void renderUI(entt::registry& scene, float viewportWidth, float viewportHeight);
    void renderUIElement(entt::registry& scene, const entt::entity& object, float viewportWidth, float viewportHeight, glm::mat4 model, glm::mat4& projection);
    void renderDepthMap(entt::registry& scene, glm::mat4& lightProjection);

    void draw(Mesh& mesh);
};

#endif //RENDERER_HPP