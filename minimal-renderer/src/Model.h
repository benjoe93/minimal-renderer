#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include <assimp/scene.h>

#include "Mesh.h"

class Mesh;
class Material;
class Texture2D;

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
    std::vector<std::unique_ptr<Mesh>> m_meshes;

    std::unordered_map<uint32_t, Material*> m_material_slots;

    glm::mat4 m_model_matrix = glm::mat4(1.0);
    bool m_matrix_dirty = true;

    void LoadModel(const std::string& path);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(const aiMesh* mesh, const aiScene* scene);
    void LoadMaterialTextures(Material* material, const aiMaterial* ai_material, aiTextureType type, const std::string& type_name);

public:
    Model(const std::string& path, const Transform& transform = Transform());
    Model(std::vector<std::unique_ptr<Mesh>> meshes, const Transform& transform = Transform());

    void SetTransform(const Transform& new_transform);
    void SetLocation(const glm::vec3& new_location);
    void SetRotation(const glm::vec3& new_rotation);
    void SetScale(const glm::vec3& new_scale);

    void SetLocationOffset(const glm::vec3& new_location);
    void SetRotationOffset(const glm::vec3& new_rotation);

    std::vector<std::unique_ptr<Mesh>>& GetMeshes() { return m_meshes; }
    const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const { return m_meshes; }

    glm::mat4 GetModelMatrix();
    const Transform& GetTransform() const { return m_transform; }
    const glm::vec3& GetLocation() const { return m_transform.Location; }
    const glm::vec3& GetRotation() const { return m_transform.Rotation; }
    const glm::vec3& GetScale() const { return m_transform.Scale; }

    // Material slot management
    void SetMaterialSlot(uint32_t slot_id, Material* material);
    Material* GetMaterialSlot(uint32_t slot_id) const;
    bool HasMaterialSlot(uint32_t slot_id) const;

    // Get all materials used by this model
    std::vector<Material*> GetAllMaterials() const;

    //Get material for a specific mesh
    Material* GetMaterialForMesh(const Mesh* mesh) const;

private:
    void UpdateModelMatrix();
};

