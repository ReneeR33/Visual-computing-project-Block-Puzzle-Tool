#ifndef SCENE_HPP
#define SCENE_HPP

#include "ModelData.hpp"
#include "TextureData.hpp"

#include <entt/entt.hpp>
#include <memory>
#include <string>
#include <map>

struct Scene {
    std::map<std::string, std::unique_ptr<ModelData>> models;
    std::map<std::string, std::unique_ptr<TextureData>> textures;

    entt::registry registry;
};

#endif //SCENE_HPP