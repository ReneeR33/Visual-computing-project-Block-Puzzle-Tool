#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string>

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
    std::vector<std::vector<glm::vec3>> file_result;
    LoaderPuzzleResult result;

    std::string line;
    std::ifstream myfile (path);
    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            if(line.length() == 0 || line[0] == '#')
            {
                continue;
            }
            file_result.push_back(LoadPiece(line));
            std::cout << line << std::endl;
        }
        myfile.close();
    }
    else 
    {
        std::cout << "Unable to open file" << std::endl;
        return result;
    } 

    glm::vec3 input_colors[] = 
    {
        glm::vec3(1.000000, 0.106535, 0.151241),
        glm::vec3(0.064279, 0.242948, 1.000000),
        glm::vec3(1.000000, 0.023907, 0.932830),
        glm::vec3(0.019089, 1.000000, 0.049019),
        glm::vec3(1.000000, 1.000000, 1.000000),
        glm::vec3(0.147655, 0.099908, 0.025475),
        glm::vec3(0.057968, 0.057968, 0.057968),
        glm::vec3(0.008985, 0.309690, 0.274033),
    };

    for (uint i = 0; i < file_result.size(); i++)
    {
        LoaderPieceResult piece;
        piece.blocks = file_result[i];
        piece.color = input_colors[i];

        result.pieces.push_back(piece);
    }

    return result;
}


std::vector<glm::vec3> ModelLoader::LoadPiece(std::string line)
{
    std::vector<glm::vec3> piece;
    std::stringstream ss(line);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> xyz(begin, end);
    
    for(uint i = 0; i < xyz.size(); i +=3)
    {
        float x = std::stoi(xyz[i]);
        float y = std::stoi(xyz[i+1]);
        float z = std::stoi(xyz[i+2]);
        piece.push_back(glm::vec3(x, y, z));
    }

    for (auto & block : piece)
    {
        std::cout << block.x << block.y << block.z << std::endl;
    }

    return piece;
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

    // result.piece = piece;
    // result.material = material;
    // result.model.meshes = meshes;
    return result;
}