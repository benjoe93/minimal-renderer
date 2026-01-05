#include "UniformBufferObject.h"
#include "Renderer.h"

UniformBufferObj::UniformBufferObj(GLsizeiptr size)
    : m_renderer_id(0), m_size(size)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    Bind();
    GLCall(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW));
    Unbind();
}

UniformBufferObj::~UniformBufferObj()
{
    if (m_renderer_id != 0)
    {
        GLCall(glDeleteBuffers(1, &m_renderer_id));
    }
}

UniformBufferObj::UniformBufferObj(UniformBufferObj&& other) noexcept
    : m_renderer_id(other.m_renderer_id),
      m_size(other.m_size)
{
    other.m_renderer_id = 0;
    other.m_size = 0;
}

UniformBufferObj& UniformBufferObj::operator=(UniformBufferObj&& other) noexcept
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
        m_size = other.m_size;

        other.m_renderer_id = 0;
        other.m_size = 0;
    }
    return *this;
}

void UniformBufferObj::Bind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_renderer_id));
}

void UniformBufferObj::Unbind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBufferObj::SetRange(GLuint idx, GLintptr offset, GLsizeiptr size) const
{
    GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, idx, m_renderer_id, offset, size));
}

void UniformBufferObj::StoreData(GLintptr offset, GLsizeiptr size, const void* data) const
{
    if (offset + size > m_size)
    {
        std::cerr << "Warning: StoreData would exceed buffer bounds!\n";
        return;
    }

    Bind();
    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
}
