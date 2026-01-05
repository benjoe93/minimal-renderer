#pragma once
#include <glad/glad.h>
#include "Texture.h"

class RenderTarget : public Texture
{
public:
    RenderTarget(GLuint width, GLuint height, GLuint nr_channels);
};