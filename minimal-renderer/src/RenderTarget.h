#pragma once
#include <iostream>
#include "Texture.h"

class RenderTarget : public Texture
{
private:

public:
    RenderTarget(int width, int height, int nr_channels, std::string sampler_name);
    ~RenderTarget();
};