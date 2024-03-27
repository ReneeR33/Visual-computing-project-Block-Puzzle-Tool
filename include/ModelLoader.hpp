#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Scene.hpp"

class ModelLoader {
public:
    void loadModel(Scene& scene, const char* filePath) const;

private:
    void processNode(aiNode *node, const aiScene *assimpScene, ModelData* model, const std::string& directory) const;
    Mesh processMesh(aiMesh *mesh, const aiScene *assimpScene, ModelData* model, const std::string& directory) const;
    /*void loadMaterialTextures(aiMaterial *mat, aiTextureType assimpType, Texture::Type type, std::vector<Texture*>& textures, 
        Model& model, const std::string& directory) const;*/
};

#endif //MODEL_LOADER_HPP