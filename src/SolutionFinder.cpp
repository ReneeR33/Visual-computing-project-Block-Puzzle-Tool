#include <entt/entt.hpp>
#include <typeinfo>
#include <string>
#include <iostream>

#include "SolutionFinder.hpp"

SolutionFinder::SolutionFinder(glm::vec3 size)
{
    uint buffer = 6;
    mapSize = glm::vec3(size.x + buffer, size.y + buffer, size.z + buffer);
    puzzleSize = glm::vec3(3);
}

SolutionFinder::~SolutionFinder()
{

}

std::vector<Solution> SolutionFinder::GetSolution(ModelLoader::LoaderPuzzleResult pieces)
{
    std::vector<Solution> result;

    for (auto & piece : pieces.pieces)
    {
        auto map = CreateMap(pieces, piece);
        Solution path = AStar(map, piece.origin);

        std::string line = "";
        for (auto & y : map[5])
        {
            for (bool z : y)
            {
                line += std::to_string(int(z)) + " ";
            }
            line += "\n";
        }
        std::cout << line << std::endl;
        break;


        if(path.Solution.size() == 0)
        {
            std::cout << "no solution found" << std::endl;
        }
        else
        {
            for (auto & step : path.Solution)
            {
                std::cout << step.x << "-" << step.y << "-" << step.x << " || ";
            }
            std::cout << std::endl;
        }
    }

    return result;
}

Solution SolutionFinder::AStar(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 start)
{
    Solution path;

    return path;
}

std::vector<std::vector<std::vector<bool>>> SolutionFinder::CreateMap(
                                ModelLoader::LoaderPuzzleResult pieces, 
                                ModelLoader::LoaderPieceResult piece)
{
    std::vector<std::vector<std::vector<bool>>> map;
    std::vector<glm::vec3> shape = piece.blocks;
    for (auto & pos : shape) {pos = pos * glm::vec3(-1.0);}

    map.resize(mapSize.x,std::vector<std::vector<bool> >(
                                    mapSize.y, std::vector<bool>(mapSize.z)));

    for (auto & otherPiece : pieces.pieces)
    {
        if (otherPiece.origin == piece.origin) {continue;}

        for (auto & block :otherPiece.blocks)
        {
            for (auto & offset :shape)
            {
                glm::vec3 pos = offset + glm::vec3(2);
                glm::vec3 loc = block + pos + glm::vec3(2);
                std::cout << block.x << " " <<  block.y << " " << block.z << std::endl;
                std::cout << offset.x << " " <<  offset.y << " " << offset.z << std::endl;
                std::cout << loc.x << " " <<  loc.y << " " << loc.z << std::endl;
                std::cout << "=========" << std::endl;
                map[loc.x][loc.y][loc.z] = true;
            }
        }

        // std::cout << step.x << "-" step.y << "-" step.x << " || ";
    }

    return map;
}