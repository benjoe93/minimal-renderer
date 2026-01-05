#pragma once
#include <glad/glad.h>

class VertexBuffer;

class VertexArray
{
private:
    GLuint m_renderer_id;

public:
    VertexArray();
    ~VertexArray();

    // Prevent copying
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    // Allow moving
    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void SetLayout(const VertexBuffer& vbo, GLuint layout, GLint size, GLenum type,
                   GLboolean normalize, GLsizei stride, const void* offset);

    void Bind() const;
    void Unbind() const;

    GLuint GetId() const { return m_renderer_id; }
};