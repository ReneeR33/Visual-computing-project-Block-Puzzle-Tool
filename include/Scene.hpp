#ifndef SCENE_HPP 
#define SCENE_HPP

#include "Object.hpp"
#include "Camera.hpp"
#include "DirLight.hpp"
#include <vector>

struct Scene {
    std::vector<Model> models;
    DirLight dirlight;
    std::vector<Object> objects;
    Camera camera;
};

#endif //SCENE_HPP