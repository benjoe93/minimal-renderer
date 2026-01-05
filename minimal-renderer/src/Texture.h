#pragma once
#include "Renderer.h"

class Texture
{
    protected:
        GLuint m_renderer_id = 0;
        GLuint m_width;
        GLuint m_height;
        GLuint m_nr_channels;

        GLenum GetFormat() const;

public:
    Texture(GLuint width, GLuint height, GLuint nr_channels);
    virtual ~Texture();

    // Prevent copying
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Move semantics
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    virtual void Bind(GLuint slot = 0) const;
    virtual void Unbind() const;

    GLuint GetId() const { return m_renderer_id; }
    GLuint GetWidth() const { return m_width; }
    GLuint GetHeight() const { return m_height; }

    void SetWrappingHorizontal(GLint new_wrapping);
    void SetWrappingVertical(GLint new_wrapping);
};
