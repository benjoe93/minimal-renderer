#pragma once
#include <glad/glad.h>

class IndexBuffer
{
private:
    GLuint m_renderer_id;
    size_t m_count;

public:
    IndexBuffer(const GLuint *data, size_t count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&) noexcept = default;
    IndexBuffer& operator=(IndexBuffer&&) noexcept = default;

    void Bind() const;
    void Unbind() const;

    GLuint GetId() const { return m_renderer_id; }
    size_t GetCount() const { return m_count; }
};