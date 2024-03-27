#include "ModelLoader.hpp"

#include <stdexcept>


void ModelLoader::loadModel(Scene& scene, const char* filePath) const {
    Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (assimpScene == nullptr) {
        throw std::runtime_error("Failed to load model");
    }

    scene.models[filePath] = std::make_unique<ModelData>();
    auto modelData = scene.models[filePath].get();

    auto filePathStr = std::string(filePath);
    auto directory = filePathStr.substr(0, filePathStr.find_last_of("/"));
    processNode(assimpScene->mRootNode, assimpScene, modelData, directory);
}

void ModelLoader::processNode(aiNode *node, const aiScene *assimpScene, ModelData* model, const std::string& directory) const {
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = assimpScene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(processMesh(mesh, assimpScene, model, directory));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], assimpScene, model, directory);
    }
}

Mesh ModelLoader::processMesh(aiMesh *mesh, const aiScene *assimpScene, ModelData* model, const std::string& directory) const {
    Mesh result;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        aiVector3D position = mesh->mVertices[i];
        vertex.position = glm::vec3(position.x, position.y, position.z);

        if (mesh->HasNormals()) {
            aiVector3D normal = mesh->mNormals[i];
            vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
        }

        if (mesh->mTextureCoords[0])
        {
            aiVector3D texCoords = mesh->mTextureCoords[0][i];
            vertex.texcoords = glm::vec2(texCoords.x, texCoords.y);
        }

        result.vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            result.indices.push_back(face.mIndices[j]); 
        }
    }

    aiMaterial* material = assimpScene->mMaterials[mesh->mMaterialIndex];

    loadMaterialTextures(material, aiTextureType_DIFFUSE, directory, model, &result);

    return result;
}

void ModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType assimpType, const std::string& directory, ModelData* model, Mesh* mesh) const {
    for(unsigned int i = 0; i < mat->GetTextureCount(assimpType); i++)
    {
        aiString str;
        mat->GetTexture(assimpType, i, &str);
        bool skip = false;

        std::string path = directory + '/' + str.C_Str();

        for(auto& texture : model->textures)
        {
            if(path == texture.path)
            {
                mesh->texture = &texture;
                skip = true; 
                break;
            }
        }
        if ( !skip )
        {
            TextureData newTexture;
            newTexture.path = path;
            model->textures.push_back(newTexture);
            mesh->texture = &model->textures.back();
        }
    }
}