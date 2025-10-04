#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include <assimp/scene.h>

#include "Mesh.h"

class Mesh;
class Material;
class Texture;

enum TextureType;

class Model
{
private:
    std::string m_directory;
    std::string m_vertex_shader_path;
    std::string m_fragment_shader_path;
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_texture_cache; // tracks loaded paths

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterialTextures(Material* material, aiMaterial* ai_material, aiTextureType type, TextureType type_name);

public:
    Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader);

    std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }
    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }
};

