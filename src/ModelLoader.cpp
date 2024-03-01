#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include "ModelLoader.hpp"
#include <typeinfo>

ModelLoader::ModelLoader()
{

}

ModelLoader::~ModelLoader()
{

}

Model ModelLoader::LoadModel(std::string path)
{
    std::ifstream check(path);
    Model model;
    Mesh mesh;

    if(!nlohmann::json::accept(check))
    {
        std:: cout << "malformed json file for " << path << std::endl;
        return model;
    }

    std::ifstream read(path);
    nlohmann::json data =  nlohmann::json::parse(read);

    if (!data.contains(std::string {"polygons"}) || 
        !data.contains(std::string {"verts"}) || 
        !data.contains(std::string {"normals"}))
    {
        std:: cout << "json file is missing info for " << path << std::endl;
        return model;
    }

    std::vector<Mesh> meshes;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < data["verts"].size(); i++) 
    {
        // deal with weird data shite of json
        auto n_0 = data["normals"][i][0].template get<double>();
        auto n_1 = data["normals"][i][1].template get<double>();
        auto n_2 = data["normals"][i][2].template get<double>();

        vertices.push_back({ .position = {data["verts"][i][0], data["verts"][i][1], data["verts"][i][2]}, 
                             .normal = {n_0, n_1, n_2 }});
    }

    for (int i = 0; i < data["polygons"].size(); i++) 
    {
        indices.push_back(data["polygons"][i]);
    }

    mesh.vertices = vertices;
    mesh.indices = indices;
    meshes.push_back(mesh);
    model.meshes = meshes;

    return model;
}