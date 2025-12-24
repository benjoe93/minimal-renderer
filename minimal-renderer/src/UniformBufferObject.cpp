#include "UniformBufferObject.h"
#include "Renderer.h"

UniformBufferObj::UniformBufferObj(GLsizeiptr size)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    Bind();
    GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

UniformBufferObj::~UniformBufferObj()
{
    Unbind();
    GLCall(glDeleteBuffers(1, &m_renderer_id));
}

void UniformBufferObj::Bind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id));
}

void UniformBufferObj::Unbind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBufferObj::SetRange(unsigned int idx, GLintptr offset, GLsizeiptr size)
{
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, idx, m_renderer_id, offset, size));
}

void UniformBufferObj::StoreData(GLintptr offset, GLsizeiptr size, const void* data)
{
    // TODO: automate the offset calculation based on provided data
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
}
