#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include "glm/glm.hpp"
#include "entt/entt.hpp"
#include "Mesh.hpp"
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

    void load(entt::registry& scene);
    void render(entt::registry& scene);

private:
    Shader fillShader;
    static Mesh fillMesh;

    void load(Mesh& mesh);

    void renderWorld(entt::registry& scene, float viewportWidth, float viewportHeight, glm::mat4 eTransform);
    void renderUI(entt::registry& scene, float viewportWidth, float viewportHeight);
    void renderUIElement(entt::registry& scene, const entt::entity& object, float viewportWidth, float viewportHeight, glm::mat4 model, glm::mat4& projection);
    void renderWorldObject(entt::registry& scene, const entt::entity& object, Camera camera, DirLight dirlight, glm::mat4& view, glm::mat4& projection, glm::mat4& eTransform);

    void draw(Mesh& mesh);
};

#endif //RENDERER_HPP