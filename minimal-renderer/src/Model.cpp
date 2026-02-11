#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "ResourceManager.h"

#include "Model.h"

#include <ranges>

#define  DEFAULT_VERT_PATH "resources/shaders/02_LoadingMesh/object.vert"
#define  DEFAULT_FRAG_PATH "resources/shaders/02_LoadingMesh/object.vert"


void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals |           // Generate normals if missing
        aiProcess_OptimizeMeshes |       // Merge small meshes
        aiProcess_JoinIdenticalVertices  // Reduce duplicate vertices

    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));
    m_meshes.reserve(scene->mNumMeshes); // pre-allocate space if we know mesh count

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::unique_ptr<Mesh> Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // pre-allocate to avoid reallocations
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    // process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z );

        // Normal
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z );
        }

        // UVs
        if (mesh->mTextureCoords[0]) { // check if there are any texture coordinates
            vertex.TexCoords =  glm::vec2( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        uint32_t slot_id = mesh->mMaterialIndex;

        // Get or create material via ResourceManager
        Material* material = ResourceManager::Get().GetMaterial(DEFAULT_VERT_PATH, DEFAULT_FRAG_PATH);

        // load textures
        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
        LoadMaterialTextures(material, ai_material, aiTextureType_DIFFUSE, "material.diffuse");
        LoadMaterialTextures(material, ai_material, aiTextureType_SPECULAR, "material.specular");

        // Assign to slot
        SetMaterialSlot(slot_id, material);

        // Set slot ID on mesh
        auto new_mesh = std::make_unique<Mesh>(std::move(vertices), std::move(indices));
        new_mesh->SetMaterialSlotId(slot_id);

        return new_mesh;
    }

    return std::make_unique<Mesh>(std::move(vertices), std::move(indices));
}

void Model::LoadMaterialTextures(Material* material, const aiMaterial* ai_material, const aiTextureType type, const std::string& type_name)
{
    const unsigned int texture_count = ai_material->GetTextureCount(type);

    for (unsigned int i = 0; i < texture_count; i++) {
        aiString current_path;
        if (ai_material->GetTexture(type, i, &current_path) != AI_SUCCESS)
            continue;

        std::string texture_path = current_path.C_Str();  // Move this outside the ifdef

#ifndef NDEBUG
        std::cout << current_path.C_Str() << std::endl;
        std::cout << texture_path << std::endl;
#endif

        if (texture_path.empty())
            continue;

        std::string full_path = m_directory + '/' + texture_path;

        Texture2D* new_texture = ResourceManager::Get().GetTexture2D(full_path);
        material->AddTexture(type_name, new_texture);
    }
}

Model::Model(const std::string& path, const Transform& transform)
    :m_transform(transform)
{
    LoadModel(path);
}

Model::Model(std::vector<std::unique_ptr<Mesh>> meshes, const Transform& transform)
    : m_transform(transform),
      m_meshes(std::move(meshes))
{}

void Model::SetTransform(const Transform &new_transform) {
    m_transform = new_transform;
    m_matrix_dirty = true;
}

void Model::SetLocation(const glm::vec3 &new_location) {
    m_transform.Location = new_location;
    m_matrix_dirty = true;
}

void Model::SetRotation(const glm::vec3 &new_rotation) {
    m_transform.Rotation = new_rotation;
    m_matrix_dirty = true;
}

void Model::SetScale(const glm::vec3 &new_scale) {
    m_transform.Scale = new_scale;
    m_matrix_dirty = true;
}

void Model::SetLocationOffset(const glm::vec3 &new_location) {
    m_transform.Location += new_location;
    m_matrix_dirty = true;
}

void Model::SetRotationOffset(const glm::vec3 &new_rotation) {
    m_transform.Rotation += new_rotation;
    m_matrix_dirty = true;
}

glm::mat4 Model::GetModelMatrix() {
    UpdateModelMatrix();
    return m_model_matrix;
}

void Model::SetMaterialSlot(uint32_t slot_id, Material* material)
{
    m_material_slots[slot_id] = material;
}

Material* Model::GetMaterialSlot(uint32_t slot_id) const
{
    if (const auto it = m_material_slots.find(slot_id); it != m_material_slots.end()) {
        return it->second;
    }
    return nullptr;  // Or return default material
}

bool Model::HasMaterialSlot(uint32_t slot_id) const
{
    return m_material_slots.contains(slot_id);
}

Material* Model::GetMaterialForMesh(const Mesh* mesh) const
{
    return GetMaterialSlot(mesh->GetMaterialSlotId());
}

std::vector<Material*> Model::GetAllMaterials() const
{
    std::vector<Material*> materials;
    materials.reserve(m_material_slots.size());


    for (const auto &material: m_material_slots | std::views::values) {
        materials.push_back(material);
    }

    return materials;
}


void Model::UpdateModelMatrix()
{
    if (!m_matrix_dirty) return;

    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_transform.Location);

    constexpr glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.x), X_AXIS);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.y), Y_AXIS);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.z), Z_AXIS);

    m_model_matrix = glm::scale(m_model_matrix, m_transform.Scale);
    m_matrix_dirty = false;
}
