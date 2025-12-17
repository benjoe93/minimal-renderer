#pragma once
#include <iostream>
#include "Texture.h"

class Texture2D : public Texture
{
private:
    std::string m_filepath;

    void LoadTexture(bool vertical_flip);

public:
    // connstuctor to read and build the texture
    Texture2D(const std::string filepath, bool vertical_flip = true);
    ~Texture2D();

    // Prevent copying (textures own GPU resources)
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    // Allow moving
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;
    
    std::string GetFilepath() const { return m_filepath; }
};