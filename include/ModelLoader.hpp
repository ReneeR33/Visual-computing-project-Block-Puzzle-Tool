#include <entt/entt.hpp>
#include "Components/Model.hpp"
#include "Components/Material.hpp"
#include "Components/PuzzlePiece.hpp"

class ModelLoader {
public:
    struct LoaderPieceResult {
        PuzzlePiece piece;
        Material material;
        Model model;
    };

    struct LoaderPuzzleResult {
        std::vector<LoaderPieceResult> pieces;
    };

    ModelLoader();
   ~ModelLoader();

    LoaderPuzzleResult LoadSolution(std::string path);
    LoaderPieceResult LoadModel(std::string path);
private:
    std::vector<Model> blocks;
    std::vector<PuzzlePiece> shape;
    std::vector<Material> colors;
};