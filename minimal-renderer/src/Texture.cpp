#include <iostream>

#include "vendor/stb_image/stb_image.h"

#include "Renderer.h"
#include "Texture.h"

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

std::string Texture::GetShaderUsage() const
{
    switch (m_type) {
    case NONE:      return "";
    case DIFFUSE:   return "material.diffuse";
    case SPECULAR:  return "material.specular";
    case NORMAL:    return "material.normal";
    case HEIGHT:    return "material.height";
    default:        return "";
    }
}

void Texture::LoadTexture(bool vertical_flip)
{
    // sets the image import to be upside down
    // this is necessary due to how stb and opengl handle coordinates
    stbi_set_flip_vertically_on_load(vertical_flip);

    GLCall(glGenTextures(1, &m_renderer_id));

    // load and generate texture
    unsigned char* data = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_nr_channels, 0);
    if (data)
    {
        // set shader name based on type (only if not already set)
        if (m_sampler_name.empty())
        {
            m_sampler_name = GetShaderUsage();
        }

        GLenum format = GetFormat();

        GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        // set texture wrapping/filtering options
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    }
    else
    {
        std::cout << "Failed to load texture: " << m_filepath << std::endl;
    }
    stbi_image_free(data);
}

Texture::Texture(const std::string& filepath, TextureType type, bool vertical_flip)
    : m_filepath(filepath),
    m_type(type),
    m_renderer_id(0),
    m_width(0),
    m_height(0),
    m_nr_channels(0),
    m_sampler_name("")
{
    LoadTexture(vertical_flip);
}

Texture::Texture(const std::string& filepath, std::string sampler_name, bool vertical_flip)
    : m_filepath(filepath),
    m_sampler_name(sampler_name),
    m_type(TextureType::NONE),
    m_renderer_id(0),
    m_width(0),
    m_height(0),
    m_nr_channels(0)
{
   LoadTexture(vertical_flip);
}

Texture::~Texture()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteTextures(1, &m_renderer_id));
    }
}

Texture::Texture(Texture&& other) noexcept
    : m_renderer_id(other.m_renderer_id),
    m_width(other.m_width),
    m_height(other.m_height),
    m_nr_channels(other.m_nr_channels),
    m_filepath(std::move(other.m_filepath)),
    m_sampler_name(std::move(other.m_sampler_name)),
    m_type(other.m_type)
{
    other.m_renderer_id = 0; // Prevent cleanup of moved resource
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resource
        if (m_renderer_id != 0)
        {
            GLCall(glDeleteTextures(1, &m_renderer_id));
        }

        // Move data
        m_renderer_id = other.m_renderer_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nr_channels = other.m_nr_channels;
        m_filepath = std::move(other.m_filepath);
        m_sampler_name = std::move(other.m_sampler_name);
        m_type = other.m_type;

        other.m_renderer_id = 0;
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
