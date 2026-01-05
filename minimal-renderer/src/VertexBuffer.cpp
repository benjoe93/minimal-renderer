#include "Renderer.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size)
    : m_renderer_id(0)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteBuffers(1, &m_renderer_id));
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_renderer_id(other.m_renderer_id)
{
    other.m_renderer_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resource
        if (m_renderer_id != 0)
        {
            GLCall(glDeleteBuffers(1, &m_renderer_id));
        }

        // Move new resource
        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }
    return *this;
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}