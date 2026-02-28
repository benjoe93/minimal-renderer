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
    if (m_renderer_id != 0)
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

void Framebuffer::AttachRenderTarget(AttachmentTarget target, RenderTarget* render_target)
{
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, static_cast<GLenum>(target), GL_TEXTURE_2D, render_target->GetId(), 0));
    render_targets[target] = render_target;
    Validate();
}

void Framebuffer::AttachRenderBuffer(AttachmentTarget target, const RenderBuffer* render_buffer)
{
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, static_cast<GLenum>(target), GL_RENDERBUFFER, render_buffer->GetId()));
    Validate();
}

RenderTarget* Framebuffer::GetRenderTarget(const AttachmentTarget target) const
{
    if (auto it = render_targets.find(target); it != render_targets.end())
        return it->second;
    return nullptr;
}

bool Framebuffer::IsComplete() const {
    GLint previous = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previous);

    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FBO error: " << status << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, previous);

    return status == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::Validate() const
{
    switch (const auto error = glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cerr << m_renderer_id << " error: Not all framebuffer attachment points are framebuffer attachment complete. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cerr << m_renderer_id << " error: No images are attached to the framebuffer. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cerr << m_renderer_id << " error: value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cerr << m_renderer_id << " error: GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cerr << m_renderer_id << " error: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        std::cerr << m_renderer_id << " error: the value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers, or the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures. " << error << '\n';
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        std::cerr << m_renderer_id << " error: any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target. " << error << '\n';
        break;
    default:
        std::cerr << m_renderer_id << " error: Unknown framebuffer error. " << error << '\n';
        break;
    }
}
