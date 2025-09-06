#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::SetLayout(const VertexBuffer& vbo, GLint layout, GLint size, GLenum type, GLboolean normalize, GLsizei stride, const void* offset)
{
    vbo.Bind();
    GLCall(glVertexAttribPointer(layout, size, type, normalize, stride, offset));
    GLCall(glEnableVertexAttribArray(layout));
    vbo.Unbind();
}


void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}