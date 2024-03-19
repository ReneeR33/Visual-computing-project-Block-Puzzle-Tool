#include <entt/entt.hpp>
#include <typeinfo>
#include <string>
#include <iostream>

#include "SolutionFinder.hpp"

SolutionFinder::SolutionFinder(glm::vec3 size)
{
    mapSize = size * glm::vec3(4);
    puzzleSize = size;
}

SolutionFinder::~SolutionFinder()
{

}

std::vector<Solution> SolutionFinder::GetSolution(ModelLoader::LoaderPuzzleResult pieces)
{
    std::vector<Solution> result;
    std::vector<glm::vec3> goalLocs;
    std::vector<bool> isSolved;
    isSolved.resize(pieces.pieces.size());
    result.resize(pieces.pieces.size());

    uint32_t solvedCount = 0;

    while(true)
    {
        auto preSolved = solvedCount;

        for(int i = 0; i < pieces.pieces.size(); i++)
        {
            if(isSolved[i]) {continue;}

            auto piece = pieces.pieces[i];

            ModelLoader::LoaderPuzzleResult otherPieces;
            for(int i = 0; i < pieces.pieces.size(); i++)
            {
                if(!isSolved[i]) otherPieces.pieces.push_back(pieces.pieces[i]);
            }
            auto map = CreateMap(otherPieces, piece);
            glm::vec3 offset = puzzleSize * glm::vec3(2);
            Solution path = AStar(map, glm::floor(piece.origin) + offset, glm::vec3(0));
            
            if(path.Solution.size() > 0)
            {
                isSolved[i] = true;
                result[i] = path;
            }
        }
        
        if(preSolved == solvedCount)
        {
            std::cout << "solved all solvable pieces." << std::endl;
            break;
        }
    }

    return result;
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

        for (auto & otherBlock :otherPiece.blocks)
        {
            glm::vec3 offset = puzzleSize * glm::vec3(2) + glm::floor(otherPiece.origin);
            for (auto & block :shape)
            {
                glm::vec3 pos = block + offset;
                glm::vec3 loc = otherBlock + block + offset;
                map[loc.x][loc.y][loc.z] = true;
            }
        }
    }

    return map;
}

// A* algorithm 

bool SolutionFinder::sort(SolutionFinder::step a, SolutionFinder::step b)
{
    return a.first < b.first;
}

float SolutionFinder::calcDist(glm::vec3 a, glm::vec3 b)
{
    return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z + b.z);
}

bool SolutionFinder::posIsValid(glm::vec3 pos)
{
    return pos.x >= 0 && pos.x < mapSize.x &&
           pos.y >= 0 && pos.y < mapSize.y &&
           pos.z >= 0 && pos.z < mapSize.z;
}

bool IsWall(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 loc)
{
    return map[loc.x][loc.y][loc.z];
}

Solution SolutionFinder::calcPath(std::vector<std::vector<std::vector<cell>>> details, glm::vec3 goal)
{
    std::vector<glm::vec3> path;
    glm::vec3 curr = goal;

    while(details[curr.x][curr.y][curr.z].parent != curr)
    {
        path.push_back(curr);
        curr = details[curr.x][curr.y][curr.z].parent;
    }

    return {.Solution = path};
}

// TODO: path takes diagonals sometimes, look into why
Solution SolutionFinder::AStar(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 start, glm::vec3 goal)
{

    Solution path;
    std::cout << "goal:" << goal.x << ":" << goal.y << ":" << goal.z << std::endl;
    std::cout << "start:" << start.x << ":" << start.y << ":" << start.z << std::endl;
    
    // Check if valid?

    if(map[start.x][start.y][start.z] || map[goal.x][goal.y][goal.z])
    {
        std::cout << "start or end not accessable" << std::endl;
        return path;
    } 
    else if(start == goal)
    {
        std::cout << "goal and start are equal" << std::endl;
        return path;
    }

    std::vector<std::vector<std::vector<bool>>> closedList;
    closedList.resize(mapSize.x,std::vector<std::vector<bool> >(
                      mapSize.y, std::vector<bool>(mapSize.z, false)));

    std::vector<std::vector<std::vector<cell>>> details;
    details.resize(mapSize.x,std::vector<std::vector<cell> >(
                   mapSize.y, std::vector<cell>(mapSize.z, 
                   {.parent = glm::vec3(-1), .dist = UINT32_MAX, 
                   .from_goal = FLT_MAX})));

    details[start.x][start.y][start.z] = {.parent = start, .dist = 0, 
                                          .from_goal = glm::distance(start, goal)};
    bool finished = false;
    std::vector<step> openList;
    openList.push_back(std::make_pair(0, start));

    while (!openList.empty()) 
    {
        step item = openList[0];
        glm::vec3 itemLoc = item.second;
        openList.erase(openList.begin());
        closedList[itemLoc.x][itemLoc.y][itemLoc.z] = true;

        std::vector<glm::vec3> neighbhours;
        neighbhours.push_back(itemLoc + glm::vec3(0, 0, 1));
        neighbhours.push_back(itemLoc + glm::vec3(0, 1, 0));
        neighbhours.push_back(itemLoc + glm::vec3(1, 0, 0));
        neighbhours.push_back(itemLoc + glm::vec3(0, 0, -1));
        neighbhours.push_back(itemLoc + glm::vec3(0, -1, 0));
        neighbhours.push_back(itemLoc + glm::vec3(-1, 0, 0));
        
        // std::cout << "neigbours for:" << itemLoc.x << ":" << itemLoc.y << ":" << itemLoc.z <<  std::endl;
        for (auto nb : neighbhours)
        {
            if (posIsValid(nb)) 
            {
                // std::cout << nb.x << ":" << nb.y << ":" << nb.z << std::endl;
                if (nb == goal) 
                {
                    details[nb.x][nb.y][nb.z].parent = itemLoc;
                    std::cout << "finished routing" << std::endl;
                    path = calcPath(details, goal);
                    finished = true;
                    break;
                }
                else if (!closedList[nb.x][nb.y][nb.z] && !IsWall(map, nb))
                {
                    float fromGoal = glm::distance(nb, goal);
                    uint32_t dist = details[itemLoc.x][itemLoc.y][itemLoc.z].dist + 1;

                    if(details[nb.x][nb.y][nb.z].from_goal == FLT_MAX ||
                    details[nb.x][nb.y][nb.z].dist > dist)
                    {
                        openList.push_back(std::make_pair(fromGoal, nb));
                        details[nb.x][nb.y][nb.z].parent = itemLoc;
                        details[nb.x][nb.y][nb.z].dist = dist;
                        details[nb.x][nb.y][nb.z].from_goal = fromGoal;
                    }
                }
            }
        }

        if(finished) {break;}
        std::sort (openList.begin(), openList.end(), sort);
    }
    return path;
}