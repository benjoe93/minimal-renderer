#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "Texture.h"

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

void Model::ProcessNode(aiNode* node, const aiScene* scene)
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

std::unique_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

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

        vertices.push_back(vertex);
    }

    // process indices
    for (unsigned int i= 0; i< mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
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
        LoadMaterialTextures(mat.get(), material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        LoadMaterialTextures(mat.get(), material, aiTextureType_SPECULAR, TextureType::SPECULAR);
    }
    

    return std::make_unique<Mesh>(
        std::move(vertices),
        std::move(indices),
        std::move(mat)
     );
}

void Model::LoadMaterialTextures(Material* material, aiMaterial* ai_material, aiTextureType type, TextureType type_name)
{
    unsigned int texture_count = ai_material->GetTextureCount(type);

    for (unsigned int i = 0; i < texture_count; i++)
    {
        aiString current_path;
        if (ai_material->GetTexture(type, i, &current_path) != AI_SUCCESS)
            continue;
        std::cout << current_path.C_Str() << std::endl;

        std::string texture_path = current_path.C_Str();

        std::cout << texture_path << std::endl;
        if (texture_path.empty())
            continue;

        std::string full_path = m_directory + '/' + texture_path;

        // check if texture already loaded
        auto it = m_texture_cache.find(texture_path);

        if (it == m_texture_cache.end())
        {
            // new texture - load it and cache the path
            std::shared_ptr<Texture> new_texture = std::make_shared<Texture>(full_path, type_name, true);
            m_texture_cache[texture_path] = new_texture;
            material->AddTexture(new_texture, type_name);
        }
        else
        {
            // texture already exists - reuse it]
            material->AddTexture(it->second, type_name);
        }
    }
}

Model::Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader)
    : m_vertex_shader_path(vertex_shader), m_fragment_shader_path(fragment_shader)
{
    LoadModel(path);
}

Model::Model(const std::string& path, const std::string& vertex_shader, const std::string& fragment_shader, Transform transform)
    : m_vertex_shader_path(vertex_shader), m_fragment_shader_path(fragment_shader), m_transform(transform)
{
    LoadModel(path);
}

glm::mat4 Model::GetModelMatrix()
{
    UpdateModelMatrix();
    return m_model_matrix;
}

void Model::UpdateModelMatrix()
{
    m_model_matrix = glm::mat4(1.0);
    m_model_matrix = glm::translate(m_model_matrix, m_transform.Location);

    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(m_transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    m_model_matrix = glm::scale(m_model_matrix, m_transform.Scale);
}
