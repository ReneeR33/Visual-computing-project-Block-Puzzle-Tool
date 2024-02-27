#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include <entt/entt.hpp>
#include "Mesh.hpp"
#include "Background.hpp"
#include "Model.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "DirLight.hpp"
#include "Shader.hpp"

class Renderer {
public:
    Renderer();

    void load(entt::registry& scene);
    void render(entt::registry& scene);

private:
    void load(Mesh& mesh);

    void render(entt::registry& scene, const entt::entity& object, Camera camera, DirLight dirlight, glm::mat4& view, glm::mat4& projection);
    void draw(Mesh& mesh);
};

#endif //RENDERER_HPP