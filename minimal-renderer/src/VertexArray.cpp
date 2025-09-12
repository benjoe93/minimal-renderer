#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_renderer_id));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_renderer_id));
}

void VertexArray::SetLayout(const VertexBuffer& vbo, int layout, int size, unsigned int type, unsigned char normalize, int stride, const void* offset)
{
    vbo.Bind();
    GLCall(glVertexAttribPointer(layout, size, type, normalize, stride, offset));
    GLCall(glEnableVertexAttribArray(layout));
    vbo.Unbind();
}


void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}