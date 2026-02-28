#include "Renderer.h"
#include "RenderTarget.h"



RenderTarget::RenderTarget(GLuint width, GLuint height, GLuint nr_channels)
    : Texture(width, height, nr_channels)
{
    GLCall(glGenTextures(1, &m_renderer_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GetFormat(), width, height, 0, GetFormat(), GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Edge clamping for render targets
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

void RenderTarget::Resize(GLuint width, GLuint height) {
    if (width == m_width && height == m_height)
        return;

    m_width = width;
    m_height = height;

    GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GetFormat(), width, height, 0, GetFormat(), GL_UNSIGNED_BYTE, nullptr));

}
