#include <cassert>
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TextureCubemap.h"
#include "Shader.h"
#include "Material.h"
#include "RenderTarget.h"
#include "RenderBuffer.h"

static ResourceManager* s_instance = nullptr;

void ResourceManager::Init()
{
    assert(!s_instance && "ResourceManager already initialized!");
    s_instance = new ResourceManager();
}

void ResourceManager::Shutdown()
{
    assert(s_instance && "ResourceManager not initialized!");
    delete s_instance;
    s_instance = nullptr;
}

ResourceManager& ResourceManager::Get()
{
    assert(s_instance && "ResourceManager not initialized! Call Init() first.");
    return *s_instance;
}

Texture2D* ResourceManager::GetTexture2D(const std::string& file_path, bool is_flipped)
{
    auto it = m_textures_2d.find(file_path);
    if (it != m_textures_2d.end())
        return it->second.get();

    auto new_texture = std::make_unique<Texture2D>(file_path, is_flipped);
    Texture2D* ptr = new_texture.get();
    m_textures_2d[file_path] = std::move(new_texture);
    return ptr;
}

TextureCubemap* ResourceManager::GetCubemap(const std::unordered_map<CubeSide, std::string>& side_source)
{
    // Create consistent key
    std::string key;
    for (const auto& [side, path] : side_source)
    {
        key += path + "|";
    }

    auto it = m_cubemaps.find(key);
    if (it != m_cubemaps.end())
        return it->second.get();

    auto new_cubemap = std::make_unique<TextureCubemap>(side_source);
    TextureCubemap* ptr = new_cubemap.get();
    m_cubemaps[key] = std::move(new_cubemap);
    return ptr;
}

Shader* ResourceManager::GetShader(const std::string& vertex_path, const std::string& fragment_path)
{
    const std::string key = vertex_path + "|" + fragment_path;

    auto it = m_shaders.find(key);
    if (it != m_shaders.end())
        return it->second.get();

    auto new_shader = std::make_unique<Shader>(vertex_path, fragment_path);
    Shader* ptr = new_shader.get();
    m_shaders[key] = std::move(new_shader);
    return ptr;
}

Material* ResourceManager::GetMaterial(const std::string& vertex_path, const std::string& fragment_path)
{
    const std::string key = vertex_path + "|" + fragment_path;

    auto it = m_materials.find(key);
    if (it != m_materials.end())
        return it->second.get();

    // Note: Material constructor gets shader from ResourceManager
    auto new_material = std::make_unique<Material>(vertex_path, fragment_path);
    Material* ptr = new_material.get();
    m_materials[key] = std::move(new_material);
    return ptr;
}

RenderTarget* ResourceManager::GetRenderTarget(const std::string& name, GLuint width, GLuint height, GLuint nr_channels)
{
    auto it = m_render_targets.find(name);
    if (it != m_render_targets.end())
        return it->second.get();

    auto new_target = std::make_unique<RenderTarget>(width, height, nr_channels);
    RenderTarget* ptr = new_target.get();
    m_render_targets[name] = std::move(new_target);
    return ptr;
}

RenderBuffer* ResourceManager::GetRenderBuffer(const std::string& name, GLuint width, GLuint height)
{
    auto it = m_render_buffers.find(name);
    if (it != m_render_buffers.end())
        return it->second.get();

    auto new_buffer = std::make_unique<RenderBuffer>(width, height);
    RenderBuffer* ptr = new_buffer.get();
    m_render_buffers[name] = std::move(new_buffer);
    return ptr;
}

void ResourceManager::RemoveTexture2D(const std::string& file_path)
{
    m_textures_2d.erase(file_path);
}

void ResourceManager::RemoveShader(const std::string& vertex_path, const std::string& fragment_path)
{
    const std::string key = vertex_path + "|" + fragment_path;
    m_shaders.erase(key);
}