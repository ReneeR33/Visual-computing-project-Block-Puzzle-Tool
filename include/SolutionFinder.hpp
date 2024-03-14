#ifndef SOLUTIONFINDER_HPP
#define SOLUTIONFINDER_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "ModelLoader.hpp"
#include "Components/Solution.hpp"


class SolutionFinder {
public:

    SolutionFinder(glm::vec3 size);
   ~SolutionFinder();

    std::vector<Solution> GetSolution(ModelLoader::LoaderPuzzleResult pieces);
private:
    Solution AStar(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 start);
    std::vector<std::vector<std::vector<bool>>> CreateMap(ModelLoader::LoaderPuzzleResult pieces, ModelLoader::LoaderPieceResult piece);

    glm::vec3 mapSize;
    glm::vec3 puzzleSize;
};

#endif //SOLUTIONFINDER_HPP