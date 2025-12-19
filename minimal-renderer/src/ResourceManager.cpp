#include <cassert>
#include "ResourceManager.h"

static ResourceManager* s_instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    // Clean up textures
    for (auto& resource : m_textures) {
        delete resource.second;
    }
    m_textures.clear();

    // Clean up materials
    for (auto& resource : m_materials) {
        delete resource.second;
    }
    m_materials.clear();

    // Clean up shaders
    for (auto& resource : m_shaders) {
        delete resource.second;
    }
    m_shaders.clear();

    // Clean up render targets
    for (auto& resource : m_render_targets) {
        delete resource.second;
    }
    m_render_targets.clear();

    // Clean up render buffers
    for (auto& resource : m_render_buffers) {
        delete resource.second;
    }
    m_render_buffers.clear();
}

void ResourceManager::Init() {
    assert(!s_instance);
    s_instance = new ResourceManager();
}

void ResourceManager::Shutdown() {
    delete s_instance;
    s_instance = nullptr;
}

ResourceManager& ResourceManager::Get()
{
    assert(s_instance);
    return *s_instance;
}

Texture2D* ResourceManager::GetTexture2D(const std::string& file_path, bool is_flipped)
{
    // Get
    auto it = m_textures.find(file_path);
    if (it != m_textures.end())
        return static_cast<Texture2D*>(it->second);

    // Create
    Texture2D* new_texture = new Texture2D(file_path, is_flipped);
    m_textures[file_path] = new_texture;
    return new_texture;
}

TextureCubemap* ResourceManager::GetCubemap(const std::unordered_map<CubeSide, std::string>& side_source)
{
    std::string key = side_source.at(CubeSide::RIGHT) + "|" +
        side_source.at(CubeSide::LEFT) + "|" +
        side_source.at(CubeSide::TOP) + "|" +
        side_source.at(CubeSide::BOTTOM) + "|" +
        side_source.at(CubeSide::FRONT) + "|" +
        side_source.at(CubeSide::BACK);

    // Get
    auto it = m_textures.find(key);
    if (it != m_textures.end())
        return static_cast<TextureCubemap*>(it->second);

    // Create
    TextureCubemap* new_texture = new TextureCubemap(side_source);
    m_textures[key] = new_texture;
    return new_texture;
}

Material* ResourceManager::GetMaterial(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string key = vertex_path + "|" + fragment_path;
    // Get
    auto it = m_materials.find(key);
    if (it != m_materials.end())
        return it->second;

    // Create
    Material* new_material = new Material(vertex_path, fragment_path);
    m_materials[key] = new_material;
    return new_material;
}

Shader* ResourceManager::GetShader(const std::string& vertex_path, const std::string& fragment_path)
{
    std::string key = vertex_path + "|" + fragment_path;
    // Get
    auto it = m_shaders.find(key);
    if (it != m_shaders.end())
        return it->second;

    // Create
    Shader* new_shader = new Shader(vertex_path, fragment_path);
    m_shaders[key] = new_shader;
    return new_shader;
}

RenderTarget* ResourceManager::GetRenderTarget(const std::string& name, unsigned int width, unsigned int height, unsigned int nr_channels)
{
    return nullptr;
}

RenderBuffer* ResourceManager::GetRenderBuffer(const std::string& name, unsigned int width, unsigned int height)
{
    return nullptr;
}
