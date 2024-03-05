#include <entt/entt.hpp>
#include "Components/Model.hpp"
#include "Components/Material.hpp"
#include "Components/Piece.hpp"

class ModelLoader {
public:
    ModelLoader(entt::registry& scene);
   ~ModelLoader();

    void LoadSolution(std::string path);
    void LoadModel(std::string path);
private:
    entt::registry& scene;
    std::vector<Model> blocks;
    std::vector<Piece> shape;
    std::vector<Material> colors;
};