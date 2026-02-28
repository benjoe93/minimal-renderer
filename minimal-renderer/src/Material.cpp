#include "Material.h"

#include <format>

#include "ResourceManager.h"
#include "Texture2D.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

Material::Material(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path)
{
    if (geometry_path.empty()) {
        m_shader = ResourceManager::Get().GetShader(vertex_path, fragment_path);
    }
    else {
        m_shader = ResourceManager::Get().GetShader(vertex_path, fragment_path, geometry_path);
    }
}

void Material::AddTexture(const std::string& sampler_name, Texture* texture)
{
    if (texture)
    {
        m_textures.emplace_back(sampler_name, texture);
    }
}

void Material::AddTexture2D(const std::string& path, const std::string& sampler_name, bool is_flipped)
{
    if (auto texture = ResourceManager::Get().GetTexture2D(path, is_flipped))
    {
        m_textures.emplace_back(sampler_name, texture);
    }
}

void Material::SetUniform(const std::string& name, float value)
{
    u_floats[name] = value;
}

void Material::SetUniform(const std::string& name, const glm::vec3& value)
{
    u_vec3[name] = value;
}

void Material::SetUniform(const std::string& name, const glm::mat4& value)
{
    u_mat4[name] = value;
}

void Material::Bind()
{
    GLint currentProgram = 0;
    m_shader->Bind();

    // Bind uniforms
    for (const auto& [name, value] : u_floats)
    {
        m_shader->SetUniform(name, value);
    }
    for (const auto& [name, value] : u_vec3)
    {
        m_shader->SetUniform(name, value.x, value.y, value.z);
    }
    for (const auto& [name, value] : u_mat4)
    {
        // std::cout << "[MATRIX] - " << name << ": " << glm::to_string(value) << std::endl;
        m_shader->SetUniform(name, value);
    }

    // Bind textures
    unsigned int slot = 0;
    for (const auto& [sampler_name, texture] : m_textures)
    {
        texture->Bind(slot);
        m_shader->SetUniform(sampler_name, static_cast<int>(slot));
        slot++;
    }
}

void Material::Unbind()
{
    for (const auto& [sampler_name, texture] : m_textures)
    {
        texture->Unbind();
    }
}

void Material::AddUniformBuffer(const std::string& block_name, UniformBufferObj* obj)
{
    m_shader->BindUniformBuffer(block_name, obj);
}