#include "ResourceManager.h"

static ResourceManager* s_instance = nullptr;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
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

Texture2D* ResourceManager::GetTexture2D(std::string file_path, bool is_flipped)
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

TextureCubemap* ResourceManager::GetCubemap(std::unordered_map<CubeSide, std::string> side_source)
{
    std::string file_path = side_source[CubeSide::FRONT];

    // Get
    auto it = m_textures.find(file_path);
    if (it != m_textures.end())
        return static_cast<TextureCubemap*>(it->second);

    // Create
    TextureCubemap* new_texture = new TextureCubemap(side_source);
    m_textures[file_path] = new_texture;
    return new_texture;
}
