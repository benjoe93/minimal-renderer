#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <glad/glad.h>

class Texture2D;
class TextureCubemap;
class Shader;
class Material;
class RenderTarget;
class RenderBuffer;
class Framebuffer;
enum class CubeSide;

class ResourceManager
{
private:
    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Separate maps for different resource types
    std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures_2d;
    std::unordered_map<std::string, std::unique_ptr<TextureCubemap>> m_cubemaps;
    std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
    std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;
    std::unordered_map<std::string, std::unique_ptr<RenderTarget>> m_render_targets;
    std::unordered_map<std::string, std::unique_ptr<RenderBuffer>> m_render_buffers;
    std::unordered_map<std::string, std::unique_ptr<Framebuffer>> m_framebuffers;

public:
    static void Init();
    static void Shutdown();
    static ResourceManager& Get();
    
    // Returns non-owning pointers - ResourceManager retains ownership
    Texture2D* GetTexture2D(const std::string& file_path, bool is_flipped = true);
    TextureCubemap* GetCubemap(const std::unordered_map<CubeSide, std::string>& side_source);
    Shader* GetShader(const std::string& vertex_path, const std::string& fragment_path);
    Shader* GetShader(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path);

    Material* CreateMaterial(const std::string& name, const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path = "");
    Material* GetMaterial(const std::string& name);
    bool IsValidMaterial(const std::string& name) const;

    RenderTarget* GetRenderTarget(const std::string& name, GLuint width, GLuint height, GLuint nr_channels);
    RenderBuffer* GetRenderBuffer(const std::string& name, GLuint width, GLuint height);
    Framebuffer* GetFramebuffer(const std::string& name);

    // Manual resource removal
    void RemoveTexture2D(const std::string& file_path);
    void RemoveShader(const std::string& vertex_path, const std::string& fragment_path);

    // Resource statistics
    size_t GetTextureCount() const { return m_textures_2d.size() + m_cubemaps.size(); }
    size_t GetShaderCount() const { return m_shaders.size(); }
};