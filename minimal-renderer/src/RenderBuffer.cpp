#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(const GLuint width, const GLuint height)
    : m_renderer_id(0), m_width(width), m_height(height)
{
    GLCall(glGenRenderbuffers(1, &m_renderer_id));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id));
    // Combined depth and stencil buffer
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
}

RenderBuffer::~RenderBuffer()
{
    if (m_renderer_id != 0)
        GLCall(glDeleteRenderbuffers(1, &m_renderer_id));
}

void RenderBuffer::Bind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id));
}

void RenderBuffer::Unbind() const
{
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}