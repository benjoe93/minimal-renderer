#include "Renderer.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, const size_t count)
    : m_count(count)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(GLuint)), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    if (m_renderer_id != 0)
        GLCall(glDeleteBuffers(1, &m_renderer_id));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}