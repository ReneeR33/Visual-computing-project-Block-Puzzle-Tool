#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <fstream>
#include <typeinfo>

#include "ModelLoader.hpp"
#include "Components/Material.hpp"
#include "Components/Transform.hpp"
#include "Components/Shader.hpp"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{

}


ModelLoader::LoaderPuzzleResult ModelLoader::LoadSolution(std::string path)
{
    LoaderPuzzleResult result;
    std::vector<LoaderPieceResult> pieces;
    pieces.push_back(LoadModel(path + "block_1.json"));
    pieces.push_back(LoadModel(path + "block_2.json"));
    pieces.push_back(LoadModel(path + "block_0.json"));
    pieces.push_back(LoadModel(path + "block_3.json"));
    pieces.push_back(LoadModel(path + "block_4.json"));
    pieces.push_back(LoadModel(path + "block_5.json"));
    pieces.push_back(LoadModel(path + "block_6.json"));
    pieces.push_back(LoadModel(path + "block_7.json"));
    auto path_puzzle = path + "puzzle_test.json";

    std::ifstream check(path_puzzle);

    if(!nlohmann::json::accept(check))
    {
        std:: cout << "malformed json file for " << path_puzzle << std::endl;
        return result;
    }

    std::ifstream read(path_puzzle);
    nlohmann::json data =  nlohmann::json::parse(read);


    for (auto & block : data["solution"])
    {
        auto x = block[1][0].template get<double>();
        auto y = block[1][1].template get<double>();
        auto z = block[1][2].template get<double>();

        auto rx = block[2][0].template get<double>();
        auto ry = block[2][1].template get<double>();
        auto rz = block[2][2].template get<double>();

        LoaderPieceResult output = pieces[block[0]];
        output.piece.initialPosition = glm::vec3(x, y, z);
        output.piece.initialRotation = glm::vec3(rx, ry, rz);

        result.pieces.push_back(output);
    }

    return result;
}


ModelLoader::LoaderPieceResult ModelLoader::LoadModel(std::string path)
{
    std::ifstream check(path);
    LoaderPieceResult result;
    Material material;
    PuzzlePiece piece;
    Mesh mesh;

    if(!nlohmann::json::accept(check))
    {
        std:: cout << "malformed json file for " << path << std::endl;
        return result;
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
        return result;
    }

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

    piece.initialPosition = {0.0, 0.0, 0.0};
    piece.selected = false;
    for (int i = 0; i < data["shape"].size(); i++) 
    {
        auto pos = data["shape"][i];
        piece.CubePos.push_back( {pos[0], pos[1],pos[2]} );
    }

    auto r = data["color"][0].template get<double>();
    auto g = data["color"][1].template get<double>();
    auto b = data["color"][2].template get<double>();
    material = {.color = {r, g, b},
                .ambientColor = {r * .1, g * .1, b * .1},
                .specularColor = glm::vec3(0.0f), 
                .specularPow = 1.0f};
    
    mesh.vertices = vertices;
    mesh.indices = indices;
    meshes.push_back(mesh);

    result.piece = piece;
    result.material = material;
    result.model.meshes = meshes;
    return result;
}