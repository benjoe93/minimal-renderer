#pragma once
#include "Renderer.h"

class RenderTarget;
class RenderBuffer;

class Framebuffer
{
private:
    unsigned int m_renderer_id;

public:
    Framebuffer();
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;

    void AttachRenderTarget(RenderTarget* render_target, unsigned int slot = 0) const;
    void AttachRenderBuffer(RenderBuffer* render_buffer) const;

    void Validate() const;
};
