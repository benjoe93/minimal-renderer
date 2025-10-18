#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height)
    : m_renderer_id(0), m_width(width), m_height(height)
{
    GLCall(glGenRenderbuffers(1, &m_renderer_id));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height)); // use a single renderbuffer object for both a depth AND stencil buffer.
}

RenderBuffer::~RenderBuffer()
{
    GLCall(glDeleteRenderbuffers(1, &m_renderer_id));
}

