#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>

#include <assimp/scene.h>

#include "Mesh.h"

class Mesh;
class Texture;
enum TextureType;

class Model
{
private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_texture_cache; // track loaded paths
    std::string m_directory;

    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterialTextures(Material* material, aiMaterial* ai_material, aiTextureType type, TextureType type_name);

public:
    Model(const char *path);

    std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }
    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }
};

