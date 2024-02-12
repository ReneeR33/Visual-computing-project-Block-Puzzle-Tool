#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Model.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <vector>

struct Object {
    Model* model;
    Shader* shader;
    glm::vec3 color;
    glm::vec3 ambientColor;
    Transform transform;
};

#endif //OBJECT_HPP