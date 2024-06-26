#ifndef SOLUTIONFINDER_HPP
#define SOLUTIONFINDER_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "PuzzleLoader.hpp"
#include "Components/Solution.hpp"


class SolutionFinder {
public:

    SolutionFinder(glm::vec3 size);
   ~SolutionFinder();

    std::vector<Solution> GetSolution(PuzzleLoader::LoaderPuzzleResult pieces);
private:
    Solution AStar(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 start, glm::vec3 goal);
    std::vector<std::vector<std::vector<bool>>> CreateMap(PuzzleLoader::LoaderPuzzleResult pieces, PuzzleLoader::LoaderPieceResult piece);

    struct cell 
    {
        glm::vec3 parent;
        uint32_t dist;
        float from_goal;
    };

    Solution calcPath(std::vector<std::vector<std::vector<cell>>> details,
                      std::vector<std::vector<std::vector<bool>>> map, 
                      glm::vec3 goal, glm::vec3 world_offset);

    typedef std::pair<float, glm::vec3> step;
    float calcDist(glm::vec3 a, glm::vec3 b);
    bool posIsValid(glm::vec3 pos);
    static bool sort(step a, step b);

    glm::vec3 mapSize;
    glm::vec3 puzzleSize;
    glm::vec3 goals[14];
};

#endif //SOLUTIONFINDER_HPP