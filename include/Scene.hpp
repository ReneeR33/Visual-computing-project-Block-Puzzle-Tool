#ifndef SCENE_HPP 
#define SCENE_HPP

#include "Object.hpp"
#include "Camera.hpp"
#include "DirLight.hpp"
#include <vector>
#include <string>
#include <map>

struct Scene {
    glm::vec3 backgroundColor;
    std::map<std::string, Model> models;
    DirLight dirLight;
    std::vector<Object> objects;
    Camera camera;
};

#endif //SCENE_HPP