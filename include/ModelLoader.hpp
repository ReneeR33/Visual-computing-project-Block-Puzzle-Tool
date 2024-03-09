#include <entt/entt.hpp>
#include "Components/Model.hpp"
#include "Components/Material.hpp"
#include "Components/PuzzlePiece.hpp"

class ModelLoader {
public:
    struct LoaderPieceResult {
        std::vector<glm::vec3> blocks;
        glm::vec3 color;
    };

    struct LoaderPuzzleResult {
        std::vector<LoaderPieceResult> pieces;
    };

    ModelLoader();
   ~ModelLoader();

    LoaderPuzzleResult LoadSolution(std::string path);
private:
    std::vector<glm::vec3> LoadPiece(std::string line);
    LoaderPieceResult LoadModel(std::string path);

    std::vector<Model> blocks;
    std::vector<PuzzlePiece> shape;
    std::vector<Material> colors;
};