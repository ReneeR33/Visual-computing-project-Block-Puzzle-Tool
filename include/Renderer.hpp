#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include "Scene.hpp"

class Renderer {
public:
    Renderer();

    void newFrame();

    void load(Scene& scene);
    void load(Mesh& mesh);

    void render(Scene& scene);

private:
    void render(Scene& scene, Object& object, glm::mat4& view, glm::mat4& projection);
    void draw(Mesh& mesh, Shader& shader);
};

#endif //RENDERER_HPP