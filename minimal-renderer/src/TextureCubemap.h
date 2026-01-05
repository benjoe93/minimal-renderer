#pragma once
#include <string>
#include <unordered_map>
#include "Texture.h"

enum class CubeSide  // Use enum class for type safety
{
    FRONT = 0,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

enum class CubePosition  // Use enum class
{
    POSITIVE_X  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    POSITIVE_Y  = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    POSITIVE_Z  = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    NEGATIVE_X  = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    NEGATIVE_Y  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    NEGATIVE_Z  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

class TextureCubemap : public Texture
{
private:
    std::unordered_map<CubeSide, std::string> m_textures_paths;

    void LoadTextureForSide(CubeSide side, CubePosition position);

public:
    TextureCubemap(const std::unordered_map<CubeSide, std::string>& side_source);

    // Prevent copying
    TextureCubemap(const TextureCubemap&) = delete;
    TextureCubemap& operator=(const TextureCubemap&) = delete;

    // Allow moving
    TextureCubemap(TextureCubemap&&) noexcept = default;
    TextureCubemap& operator=(TextureCubemap&&) noexcept = default;

    void Bind(GLuint slot = 0) const override;
    void Unbind() const override;
};