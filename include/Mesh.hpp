#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "Vertex.hpp"
#include "TextureData.hpp"

struct Mesh {
    unsigned int VAO, VBO, EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    TextureData* texture;
    TextureData* normalMapTexture;
};

#endif //MESH_HPP