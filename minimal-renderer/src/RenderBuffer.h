#pragma once
#include "Renderer.h"

class RenderBuffer
{
private:
    GLuint m_renderer_id;
    GLuint m_width;
    GLuint m_height;

public:
    RenderBuffer(GLuint width, GLuint height);
    ~RenderBuffer();

    RenderBuffer(const RenderBuffer&) = delete;
    RenderBuffer& operator=(const RenderBuffer&) = delete;
    RenderBuffer(RenderBuffer&&) noexcept = default;
    RenderBuffer& operator=(RenderBuffer&&) noexcept = default;

    void Bind() const;
    void Unbind() const;

    GLuint GetId() const { return m_renderer_id; }
    GLuint GetWidth() const { return m_width; }
    GLuint GetHeight() const { return m_height; }
};