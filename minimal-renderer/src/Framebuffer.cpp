#include "RenderTarget.h"
#include "RenderBuffer.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
    GLCall(glGenFramebuffers(1, &m_renderer_id));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));
}

Framebuffer::~Framebuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_renderer_id));
}


void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::AttachRenderTarget(RenderTarget* render_target, unsigned int slot) const
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, render_target->GetId(), 0));
}

void Framebuffer::AttachRenderBuffer(RenderBuffer* render_buffer) const
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer->GetId()); // now actually attach it
}

void Framebuffer::Validate() const
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}