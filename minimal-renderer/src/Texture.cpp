#include "Renderer.h"
#include "Texture.h"


Texture::Texture(unsigned int width, unsigned int height, unsigned int nr_channels, std::string& sampler_name)
    : m_width(width), m_height(height), m_nr_channels(nr_channels), m_sampler_name(sampler_name)
{
}

Texture::~Texture()
{
}

Texture::Texture(Texture&& other) noexcept
    : m_renderer_id(other.m_renderer_id),
    m_width(other.m_width),
    m_height(other.m_height),
    m_nr_channels(other.m_nr_channels),
    m_sampler_name(std::move(other.m_sampler_name))
{
    other.m_renderer_id = 0; // Prevent cleanup of moved resource
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        m_renderer_id = other.m_renderer_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nr_channels = other.m_nr_channels;
        m_sampler_name = std::move(other.m_sampler_name);

        other.m_renderer_id = 0; // Prevent cleanup of moved resource
    }
    return *this;
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

GLenum Texture::GetFormat() const
{
    switch (m_nr_channels)
    {
    case 1:  return GL_RED;
    case 3:  return GL_RGB;
    case 4:  return GL_RGBA;
    default: return GL_RGB;
    }
}

void Texture::SetWrappingHorizontal(const GLint new_wrapping)
{
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, new_wrapping));
}

void Texture::SetWrappingVertical(const GLint new_wrapping)
{
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, new_wrapping));
}