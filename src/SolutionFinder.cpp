#include <entt/entt.hpp>
#include <typeinfo>
#include <string>
#include <iostream>

#include "SolutionFinder.hpp"

SolutionFinder::SolutionFinder(glm::vec3 size)
{
    puzzleSize = size;

    auto x = std::max(size.x*2-1, (float)1.0);
    auto y = std::max(size.y*2-1, (float)1.0);
    auto z = std::max(size.z*2-1, (float)1.0);

    mapSize.x = std::max(size.x*2, (float)2.0);
    mapSize.y = std::max(size.y*2, (float)2.0);
    mapSize.z = std::max(size.z*2, (float)2.0);

    // quickly define areas where to yeet  blocks off to
    goals[0] = (glm::vec3(0, 0, 0));
    goals[1] = (glm::vec3(x, 0, 0));
    goals[2] = (glm::vec3(0, y, 0));
    goals[3] = (glm::vec3(0, 0, z));
    goals[4] = (glm::vec3(x, y, z));
    goals[5] = (glm::vec3(x, y, 0));
    goals[6] = (glm::vec3(x, 0, z));
    goals[7] = (glm::vec3(0, y, z));
    goals[8] = (glm::vec3(0, floor(y/2), floor(z/2)));
    goals[9] = (glm::vec3(x, floor(y/2), floor(z/2)));
    goals[10] = (glm::vec3(floor(x/2), 0, floor(z/2)));
    goals[11] = (glm::vec3(floor(x/2), y, floor(z/2)));
    goals[12] = (glm::vec3(floor(x/2), floor(y/2), 0));
    goals[13] = (glm::vec3(floor(x/2), floor(y/2), z));
}

SolutionFinder::~SolutionFinder()
{

}

std::vector<Solution> SolutionFinder::GetSolution(PuzzleLoader::LoaderPuzzleResult pieces)
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

        for(uint32_t i = 0; i < pieces.pieces.size(); i++)
        {
            if(isSolved[i]) {continue;}

            auto piece = pieces.pieces[i];

            PuzzleLoader::LoaderPuzzleResult otherPieces;
            for(uint32_t i = 0; i < pieces.pieces.size(); i++)
            {
                if(!isSolved[i]) otherPieces.pieces.push_back(pieces.pieces[i]);
            }

            auto map = CreateMap(otherPieces, piece);
            glm::vec3 offset = puzzleSize;
            Solution path = AStar(map, piece.origin + offset, goals[i % 14]);
            
            if(path.Solution.size() > 1) { isSolved[i] = true; }
            result[i] = path;
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
                                PuzzleLoader::LoaderPuzzleResult pieces, 
                                PuzzleLoader::LoaderPieceResult piece)
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
            glm::vec3 offset = puzzleSize + glm::floor(otherPiece.origin);
            for (auto & block :shape)
            {
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

Solution SolutionFinder::calcPath(std::vector<std::vector<std::vector<cell>>> details, glm::vec3 goal, glm::vec3 world_offset)
{
    std::vector<glm::vec3> path;
    glm::vec3 curr = goal;

    while(details[curr.x][curr.y][curr.z].parent != curr)
    {
        path.push_back(curr);
        curr = details[curr.x][curr.y][curr.z].parent;
    }
    path.push_back(curr);

    // translate back into puzzle coordinates
    glm::vec3 offset = puzzleSize;
    for (auto & step : path)
    {
        step -= (offset - world_offset);
    }

    std::reverse(path.begin(), path.end());
    return {.Solution = path, .step = 0};
}

// TODO: path takes diagonals sometimes, look into why
Solution SolutionFinder::AStar(std::vector<std::vector<std::vector<bool>>> map, glm::vec3 start, glm::vec3 goal)
{

    Solution path;
    path.step = 0;
    glm::vec3 world_offset = start - glm::floor(start);
    start = glm::floor(start);

    if(!posIsValid(start) || !posIsValid(goal))
    {
        std::cout << "start or end outside map" << std::endl;
        path.Solution.push_back(start - puzzleSize + world_offset);
        return path;
    } 
    else if(map[start.x][start.y][start.z] || map[goal.x][goal.y][goal.z])
    {
        std::cout << "start or end not accessable" << std::endl;
        path.Solution.push_back(start - puzzleSize + world_offset);
        return path;
    } 
    else if(start == goal)
    {
        std::cout << "goal and start are equal" << std::endl;
        path.Solution.push_back(start - puzzleSize + world_offset);
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

    std::vector<glm::vec3> test;
    test.push_back(openList[0].second + glm::vec3(0, 0, 1));
    test.push_back(openList[0].second  + glm::vec3(0, 1, 0));
    test.push_back(openList[0].second  + glm::vec3(1, 0, 0));
    test.push_back(openList[0].second  + glm::vec3(0, 0, -1));
    test.push_back(openList[0].second  + glm::vec3(0, -1, 0));
    test.push_back(openList[0].second  + glm::vec3(-1, 0, 0));
    
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
        
        for (auto nb : neighbhours)
        {
            if (posIsValid(nb)) 
            {
                if (nb == goal) 
                {
                    details[nb.x][nb.y][nb.z].parent = itemLoc;
                    path = calcPath(details, goal, world_offset);
                    finished = true;
                    break;
                }
                else if (!closedList[nb.x][nb.y][nb.z] && !IsWall(map, nb))
                {
                    float fromGoal = glm::distance(nb, goal);
                    uint32_t dist = details[itemLoc.x][itemLoc.y][itemLoc.z].dist + 1;

                    if(details[nb.x][nb.y][nb.z].from_goal == FLT_MAX)
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