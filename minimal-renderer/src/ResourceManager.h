#include <unordered_map>

#include "Material.h"
#include "RenderBuffer.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "Texture.h"
#include "Texture2D.h"
#include "TextureCubemap.h"

class ResourceManager
{
private:
    ResourceManager();
    ~ResourceManager();

    // Prevent copying
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    std::unordered_map<std::string, Texture*> m_textures;
    std::unordered_map<std::string, Material*> m_materials;
    std::unordered_map<std::string, Shader*> m_shaders;
    std::unordered_map<std::string, RenderTarget*> m_render_targets;
    std::unordered_map<std::string, RenderBuffer*> m_render_buffers;

public:
    static void Init();
    static void Shutdown();
    static ResourceManager& Get();
    
    Texture2D* GetTexture2D(const std::string& file_path, bool is_flipped = true);
    TextureCubemap* GetCubemap(const std::unordered_map<CubeSide, std::string>& side_source);

    Material* GetMaterial(const std::string& vertex_path, const std::string& fragment_path);

    Shader* GetShader(const std::string& vertex_path, const std::string& fragment_path);

    RenderTarget* GetRenderTarget(const std::string& name, unsigned int width, unsigned int height, unsigned int nr_channels);
    RenderBuffer* GetRenderBuffer(const std::string& name, unsigned int width, unsigned int height);
};