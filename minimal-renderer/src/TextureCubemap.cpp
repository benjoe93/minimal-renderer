#include <string>
#include <unordered_map>
#include <iostream>

#include <vendor/stb_image/stb_image.h>

#include "TextureCubeMap.h"
#include "Renderer.h"

TextureCubemap::TextureCubemap(const std::unordered_map<CubeSide, std::string>& side_source)
    : Texture(0, 0, 0), m_textures_paths(side_source)
{
    GLCall(glGenTextures(1, &m_renderer_id));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id));

    LoadTextureForSide(CubeSide::RIGHT, CubePosition::POSITIVE_X);
    LoadTextureForSide(CubeSide::LEFT, CubePosition::NEGATIVE_X);

    LoadTextureForSide(CubeSide::TOP, CubePosition::POSITIVE_Y);
    LoadTextureForSide(CubeSide::BOTTOM, CubePosition::NEGATIVE_Y);

    LoadTextureForSide(CubeSide::FRONT, CubePosition::POSITIVE_Z);
    LoadTextureForSide(CubeSide::BACK, CubePosition::NEGATIVE_Z);

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

void TextureCubemap::Bind(GLuint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id));
}

void TextureCubemap::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void TextureCubemap::LoadTextureForSide(CubeSide side, CubePosition position)
{
    const auto it = m_textures_paths.find(side);
    if (it == m_textures_paths.end())
    {
        std::cerr << "Warning: No texture path for cube side " << static_cast<int>(side) << '\n';
        return;
    }

    int width, height, nr_channels;
    unsigned char* data = stbi_load(it->second.c_str(), &width, &height, &nr_channels, 0);

    if (!data)
    {
        std::cerr << "Failed to load cubemap texture: " << it->second << '\n';
        return;
    }

    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid cubemap texture dimensions: " << it->second << '\n';
        stbi_image_free(data);
        return;
    }

    // Store dimensions from first face loaded
    if (m_width == 0)
    {
        m_width = static_cast<GLuint>(width);
        m_height = static_cast<GLuint>(height);
        m_nr_channels = static_cast<GLuint>(nr_channels);
    }

    GLenum format = (nr_channels == 4) ? GL_RGBA : GL_RGB;

    GLCall(glTexImage2D(static_cast<GLenum>(position), 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data ));

    stbi_image_free(data);
}