#include <iostream>

#include "vendor/stb_image/stb_image.h"

#include "Renderer.h"
#include "Texture2D.h"


void Texture2D::LoadTexture(bool vertical_flip)
{
    // sets the image import to be upside down
    // this is necessary due to how stb and opengl handle coordinates
    stbi_set_flip_vertically_on_load(vertical_flip);

    GLCall(glGenTextures(1, &m_renderer_id));

    // load and generate texture
    int width, height, nr_channels;
    unsigned char* data = stbi_load(m_filepath.c_str(), &width, &height, &nr_channels, 0);
    if (data)
    {
        m_width = width;
        m_height = height;
        m_nr_channels = nr_channels;

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

Texture2D::Texture2D(const std::string filepath, std::string sampler_name, bool vertical_flip)
    : Texture(0, 0, 0, sampler_name),
    m_filepath(filepath)
{
   LoadTexture(vertical_flip);
}

Texture2D::~Texture2D()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteTextures(1, &m_renderer_id));
    }
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
