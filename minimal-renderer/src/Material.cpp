#include "Material.h"
#include "ResourceManager.h"

Material::Material(std::string vertex_path, std::string fragment_path)
{
    m_shader = ResourceManager::Get().GetShader(vertex_path, fragment_path);
}

Material::~Material()
{
}

void Material::AddTexture(std::string sampler_name, Texture* texture)
{
    if (texture)
    {
        m_textures.push_back({
            sampler_name,
            texture
        });
    }
}

void Material::AddTexture2D(std::string path, std::string sampler_name, bool is_flipped)
{
    m_textures.push_back({
        sampler_name,
        ResourceManager::Get().GetTexture2D(path, is_flipped)
    });
}

void Material::SetUniform(const std::string type, float value)
{
    u_floats[type] = value;
}

void Material::SetUniform(const std::string type, const glm::vec3& value)
{
    u_vec3[type] = value;
}

void Material::SetUniform(const std::string type, const glm::mat4& value)
{
    u_mat4[type] = value;
}

void Material::Bind()
{
    // Bind shader
    m_shader->Bind();

    // Bind uniforms
    for (const auto& [name, value] : u_floats)
    {
        m_shader->SetUniform(name, value);
    }
    for (const auto& [name, value] : u_vec3)
    {
        m_shader->SetUniform(name, value[0], value[1], value[2]);
    }
    for (const auto& [name, value] : u_mat4) {
        m_shader->SetUniform(name, value);
    }

    // Bind textures
    for (int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i].second->Bind(i);
        m_shader->SetUniform(m_textures[i].first, i);
    }
}

void Material::Unbind()
{
    for (int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i].second->Unbind();
    }
}

void Material::AddUniformBuffer(const std::string& block_name, UniformBufferObj* obj)
{
    m_shader->BindUniformBuffer(block_name, obj);
}