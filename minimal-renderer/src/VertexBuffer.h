#pragma once
#include <glad/glad.h>

class VertexBuffer
{
private:
    GLuint m_renderer_id;

public:
    VertexBuffer(const void* data, GLsizeiptr size);
    ~VertexBuffer();

    // Prevent copying
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    // Allow moving
    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void Bind() const;
    void Unbind() const;

    GLuint GetId() const { return m_renderer_id; }
};