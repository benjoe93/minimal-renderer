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

struct Transform
{
    glm::vec3 Location = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale    = glm::vec3(1.0f);
};

class Model
{
private:
    Transform m_transform;
    std::string m_directory;
    std::string m_vertex_shader_path;
    std::string m_fragment_shader_path;
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_texture_cache; // tracks loaded paths

    glm::mat4 m_model_matrix = glm::mat4(1.0);

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    void LoadMaterialTextures(Material* material, aiMaterial* ai_material, aiTextureType type, TextureType type_name);

public:
    Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader);
    Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader, Transform transform);

    inline void SetTransform(Transform new_transform) { m_transform = new_transform; }
    inline void SetLocation(glm::vec3 new_location) { m_transform.Location = new_location; }
    inline void SetRotation(glm::vec3 new_rotation) { m_transform.Rotation = new_rotation; }
    inline void SetScale(glm::vec3 new_scale) { m_transform.Scale = new_scale; }

    inline void SetLocationOffset(glm::vec3 new_location) { m_transform.Location += new_location; }
    inline void SetRotationOffset(glm::vec3 new_rotation) { m_transform.Rotation += new_rotation; }

    std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }
    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }

    glm::mat4 GetModelMatrix();
    inline Transform GetTransform() const { return m_transform; }
    inline glm::vec3 GetLocation() const { return m_transform.Location; }
    inline glm::vec3 GetRotation() const { return m_transform.Rotation; }
    inline glm::vec3 GetScale() const { return m_transform.Scale; }

private:
    void UpdateModelMatrix();
};

