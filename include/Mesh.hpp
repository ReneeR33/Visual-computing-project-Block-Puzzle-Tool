#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "Vertex.hpp"

struct Mesh {
    unsigned int VAO;
    unsigned int VBO; 
    unsigned int EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

#endif //MESH_HPP