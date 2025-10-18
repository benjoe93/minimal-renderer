#pragma once
#include "Renderer.h"

class RenderBuffer
{
private:
    unsigned int m_renderer_id;
    unsigned int m_width;
    unsigned int m_height;

public:
    RenderBuffer(unsigned int width, unsigned int height);
    ~RenderBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetId() const { return m_renderer_id; }
};