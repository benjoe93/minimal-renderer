#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "ResourceManager.h"

#include "Model.h"


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
        if (mesh->HasNormals())
        {
            vertex.Normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z );
        }

        // UVs
        if (mesh->mTextureCoords[0]) // check if there are any texture coordinates
        {
            vertex.TexCoords =  glm::vec2( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(vertex);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    auto mat = std::make_unique<Material>(m_vertex_shader_path.c_str(), m_fragment_shader_path.c_str());

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        LoadMaterialTextures(mat.get(), material, aiTextureType_DIFFUSE, "material.diffuse");
        LoadMaterialTextures(mat.get(), material, aiTextureType_SPECULAR, "material.specular");
    }
    

    return std::make_unique<Mesh>(
        std::move(vertices),
        std::move(indices),
        std::move(mat)
     );
}

void Model::LoadMaterialTextures(Material* material, const aiMaterial* ai_material, const aiTextureType type, const std::string& type_name)
{
    const unsigned int texture_count = ai_material->GetTextureCount(type);

    for (unsigned int i = 0; i < texture_count; i++)
    {
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

        // check if texture already loaded
        if (auto it = m_texture_cache.find(texture_path); it == m_texture_cache.end())
        {
            // new texture - load it and cache the path
            Texture2D* new_texture = ResourceManager::Get().GetTexture2D(full_path);
            m_texture_cache[texture_path] = new_texture;
            material->AddTexture(type_name, new_texture);
        }
        else
        {
            // texture already exists - reuse it
            material->AddTexture(type_name, it->second);
        }
    }
}

Model::Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader)
    : m_vertex_shader_path(vertex_shader), m_fragment_shader_path(fragment_shader)
{
    LoadModel(path);
}

Model::Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader, const Transform& transform)
    :m_transform(transform),
    m_vertex_shader_path(vertex_shader),
    m_fragment_shader_path(fragment_shader)
{
    LoadModel(path);
}

Model::Model(std::unique_ptr<Mesh> mesh, const Transform& transform)
    : m_transform(transform)
{
    m_vertex_shader_path = mesh->GetMaterial().GetVertexPath();
    m_fragment_shader_path = mesh->GetMaterial().GetFragmentPath();
    m_meshes.push_back(std::move(mesh));
}

glm::mat4 Model::GetModelMatrix()
{
    UpdateModelMatrix();
    return m_model_matrix;
}

std::unordered_set<Material*> Model::GetMaterials() const
{
    std::unordered_set<Material*> materials;

    for (const auto& m : m_meshes)
    {
        materials.insert(&m->GetMaterial());
    }

    return materials;
}

void Model::SetMaterials()
{
    std::cout << "Set Materials Need to be fixed.";
}

void Model::UpdateModelMatrix()
{
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_transform.Location);

    constexpr glm::vec3 X_AXIS(1.0f, 0.0f, 0.0f);
    constexpr glm::vec3 Y_AXIS(0.0f, 1.0f, 0.0f);
    constexpr glm::vec3 Z_AXIS(0.0f, 0.0f, 1.0f);

    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.x), X_AXIS);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.y), Y_AXIS);
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.z), Z_AXIS);

    m_model_matrix = glm::scale(m_model_matrix, m_transform.Scale);
}
