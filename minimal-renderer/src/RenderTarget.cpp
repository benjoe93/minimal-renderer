#include "Renderer.h"
#include "RenderTarget.h"



RenderTarget::RenderTarget(unsigned int width, unsigned int height, unsigned int nr_channels)
    : Texture(width, height, nr_channels)
{
    GLCall(glGenTextures(1, &m_renderer_id));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GetFormat(), width, height, 0, GetFormat(), GL_UNSIGNED_BYTE, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}