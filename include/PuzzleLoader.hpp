#ifndef PUZZLELOADER_HPP
#define PUZZLELOADER_HPP

#include <entt/entt.hpp>
#include "Components/Model.hpp"
#include "Components/Material.hpp"
#include "Components/PuzzlePiece.hpp"

class PuzzleLoader {
public:
    struct LoaderPieceResult {
        std::vector<glm::vec3> blocks;
        glm::vec3 origin;
        glm::vec3 color;
    };

    struct LoaderPuzzleResult {
        std::vector<LoaderPieceResult> pieces;
    };

    PuzzleLoader();
   ~PuzzleLoader();

    LoaderPuzzleResult LoadSolution(std::string path);
    glm::vec3 LoadSize(std::string path);
private:
    LoaderPieceResult LoadPiece(std::string line, glm::vec3 size);
    LoaderPieceResult LoadModel(std::string path);

    std::vector<Model> blocks;
    std::vector<PuzzlePiece> shape;
    std::vector<Material> colors;
};

#endif //PUZZLELOADER_HPP