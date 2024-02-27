#ifndef RENDERER_HPP
#define RENDERER_HPP

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
    void load(Mesh& mesh);

    void render(entt::registry& scene, const entt::entity& object, Camera camera, DirLight dirlight, glm::mat4& view, glm::mat4& projection);
    void draw(Mesh& mesh);
};

#endif //RENDERER_HPP