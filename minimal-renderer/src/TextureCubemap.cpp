#include <string>
#include <unordered_map>

#include <vendor/stb_image/stb_image.h>

#include "TextureCubeMap.h"
#include "Renderer.h"

TextureCubemap::TextureCubemap(std::unordered_map<CubeSide, std::string> side_source)
    :Texture(0, 0, 0), m_textures_paths(side_source)
{
    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_renderer_id);

    LoadTextureForSide(CubeSide::RIGHT, CubePosition::POSITIVE_X);
    LoadTextureForSide(CubeSide::LEFT, CubePosition::NEGATIVE_X);

    LoadTextureForSide(CubeSide::TOP, CubePosition::POSITIVE_Y);
    LoadTextureForSide(CubeSide::BOTTOM, CubePosition::NEGATIVE_Y);

    LoadTextureForSide(CubeSide::FRONT, CubePosition::POSITIVE_Z);
    LoadTextureForSide(CubeSide::BACK, CubePosition::NEGATIVE_Z);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubemap::Bind(unsigned int slot) const
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
    int width, height, nr_channels;
    unsigned char* data = stbi_load(m_textures_paths[side].c_str(), &width, &height, &nr_channels, 0);
    glTexImage2D(static_cast<unsigned int>(position), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}
