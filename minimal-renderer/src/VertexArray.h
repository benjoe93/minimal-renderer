#pragma once

class VertexBuffer;

class VertexArray
{
private:
    unsigned int m_renderer_id;

public:
    VertexArray();
    ~VertexArray();

    void SetLayout(const VertexBuffer& vbo, unsigned int layout, int size, unsigned int type, unsigned char normalize, int stride, const void* offset);

    void Bind() const; 
    void Unbind() const;
};
