#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
    : m_renderer_id(0)
{
    GLCall(glGenVertexArrays(1, &m_renderer_id));
}

VertexArray::~VertexArray()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteVertexArrays(1, &m_renderer_id));
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_renderer_id(other.m_renderer_id)
{
    other.m_renderer_id = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing resource
        if (m_renderer_id != 0)
        {
            GLCall(glDeleteVertexArrays(1, &m_renderer_id));
        }

        // Move new resource
        m_renderer_id = other.m_renderer_id;
        other.m_renderer_id = 0;
    }
    return *this;
}

void VertexArray::SetLayout(const VertexBuffer& vbo, GLuint layout, GLint size, GLenum type, GLboolean normalize, GLsizei stride, const void* offset)
{
    Bind();
    vbo.Bind();
    GLCall(glVertexAttribPointer(layout, size, type, normalize, stride, offset));
    GLCall(glEnableVertexAttribArray(layout));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}