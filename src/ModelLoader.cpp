#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include "ModelLoader.hpp"
#include "Components/Material.hpp"
#include "Components/Transform.hpp"
#include "Components/Shader.hpp"

ModelLoader::ModelLoader(entt::registry& scene)
: scene(scene) 
{
    LoadModel("data/block_0.json");
    LoadModel("data/block_1.json");
    LoadModel("data/block_2.json");
    LoadModel("data/block_3.json");
    LoadModel("data/block_4.json");
    LoadModel("data/block_5.json");
    LoadModel("data/block_6.json");
    LoadModel("data/block_7.json");
}

ModelLoader::~ModelLoader()
{

}


void ModelLoader::LoadSolution(std::string path)
{
    std::ifstream check(path);

    if(!nlohmann::json::accept(check))
    {
        std:: cout << "malformed json file for " << path << std::endl;
        return;
    }

    std::ifstream read(path);
    nlohmann::json data =  nlohmann::json::parse(read);

    for (auto & block : data["solution"])
    {
        auto pos = block[1];

        auto output = scene.create();
        scene.emplace<Model>(output, blocks[block[0]]);
        scene.emplace<Piece>(output, shape[0]);
        scene.emplace<Material>(output, colors[block[0]]);
        scene.emplace<Shader>(output, "shaders/shader.vert", "shaders/shader.frag");

        auto x = block[1][0].template get<double>();
        auto y = block[1][1].template get<double>();
        auto z = block[1][2].template get<double>();

        auto rx = block[2][0].template get<double>();
        auto ry = block[2][1].template get<double>();
        auto rz = block[2][2].template get<double>();

        scene.emplace<Transform>(output,
            glm::vec3(x, y, z),
            glm::vec3(rx, ry, rz),
            glm::vec3(1.0f, 1.0f, 1.0f)
        );
    }
}


void ModelLoader::LoadModel(std::string path)
{
    std::ifstream check(path);
    Model model;
    Mesh mesh;
    Piece piece;

    if(!nlohmann::json::accept(check))
    {
        std:: cout << "malformed json file for " << path << std::endl;
        return;
    }

    std::ifstream read(path);
    nlohmann::json data =  nlohmann::json::parse(read);

    if (!data.contains(std::string {"polygons"}) || 
        !data.contains(std::string {"verts"}) || 
        !data.contains(std::string {"shape"}) || 
        !data.contains(std::string {"color"}) || 
        !data.contains(std::string {"normals"}))
    {
        std:: cout << "json file is missing info for " << path << std::endl;
        return;
    }

    std::vector<Piece> test;
    std::vector<Mesh> meshes;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < data["verts"].size(); i++) 
    {
        // deal with weird data shite of json
        auto pos = data["verts"][i];
        auto n_0 = data["normals"][i][0].template get<double>();
        auto n_1 = data["normals"][i][1].template get<double>();
        auto n_2 = data["normals"][i][2].template get<double>();

        vertices.push_back({ .position = {pos[0], pos[1],pos[2]}, .normal = {n_0, n_1, n_2 }});
    }

    for (int i = 0; i < data["polygons"].size(); i++) 
    {
        indices.push_back(data["polygons"][i]);
    }

    piece.SolutionPosition = {0.0, 0.0, 0.0};
    piece.selected = false;
    for (int i = 0; i < data["shape"].size(); i++) 
    {
        auto pos = data["shape"][i];
        piece.CubePos.push_back( {pos[0], pos[1],pos[2]} );
    }

    auto r = data["color"][0].template get<double>();
    auto g = data["color"][1].template get<double>();
    auto b = data["color"][2].template get<double>();
    colors.push_back({.color = {r, g, b},
                      .ambientColor = {r * .1, g * .1, b * .1},
                      .specularColor = glm::vec3(0.0f), 
                      .specularPow = 1.0f});
    
    mesh.vertices = vertices;
    mesh.indices = indices;
    meshes.push_back(mesh);
    model.meshes = meshes;

    blocks.push_back(model);
    shape.push_back(piece);
}