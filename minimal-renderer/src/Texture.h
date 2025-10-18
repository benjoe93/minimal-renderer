#pragma once
#include "Renderer.h"

class Texture
{
protected:
    unsigned int m_renderer_id = 0;
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_nr_channels;
    
    std::string m_sampler_name;

    GLenum GetFormat() const;

public:
    Texture(unsigned int width, unsigned int height, unsigned int nr_channels, std::string sampler_name);
    ~Texture();

    // move 
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    // prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;


    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    unsigned int GetId() const { return m_renderer_id; }
    unsigned int GetWidht() const { return m_width; }
    unsigned int GetHeight() const { return m_height; }
    std::string GetSamplerName() const { return m_sampler_name; }

    void SetWrappingHorizontal(const GLint new_wrapping);
    void SetWrappingVertical(const GLint new_wrapping);
};