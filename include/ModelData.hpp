#ifndef MODEL_DATA_HPP
#define MODEL_DATA_HPP

#include <vector>
#include <memory>
#include <list>
#include "Mesh.hpp"
#include "TextureData.hpp"

struct ModelData {
    std::vector<Mesh> meshes;
    std::list<TextureData> textures;
};

#endif //MODEL_DATA_HPP