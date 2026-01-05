#include <iostream>

#include "vendor/stb_image/stb_image.h"

#include "Renderer.h"
#include "Texture2D.h"


void Texture2D::LoadTexture(bool vertical_flip)
{
    stbi_set_flip_vertically_on_load(vertical_flip);

    GLCall(glGenTextures(1, &m_renderer_id));

    int width, height, nr_channels;
    unsigned char* data = stbi_load(m_filepath.c_str(), &width, &height, &nr_channels, 0);

    if (!data)
    {
        std::cerr << "Failed to load texture: " << m_filepath << '\n';
        GLCall(glDeleteTextures(1, &m_renderer_id));
        m_renderer_id = 0;  // Mark as invalid
        return;
    }

    // Validate dimensions
    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid texture dimensions for: " << m_filepath << '\n';
        stbi_image_free(data);
        GLCall(glDeleteTextures(1, &m_renderer_id));
        m_renderer_id = 0;
        return;
    }

    m_width = static_cast<GLuint>(width);
    m_height = static_cast<GLuint>(height);
    m_nr_channels = static_cast<GLuint>(nr_channels);

    const GLenum format = GetFormat();

    GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    stbi_image_free(data);
}

Texture2D::Texture2D(const std::string& filepath, bool vertical_flip)
    : Texture(0, 0, 0),
      m_filepath(filepath)
{
    LoadTexture(vertical_flip);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : Texture(std::move(other)),  // Call base class move constructor
    m_filepath(std::move(other.m_filepath))
{
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other)
    {
        // Call base class move assignment
        Texture::operator=(std::move(other));

        // Move derived class data
        m_filepath = std::move(other.m_filepath);
    }
    return *this;
}
