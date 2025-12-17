#pragma once
#include <iostream>
#include "Texture.h"

class RenderTarget : public Texture
{
public:
    RenderTarget(unsigned int width, unsigned int height, unsigned int nr_channels);
};