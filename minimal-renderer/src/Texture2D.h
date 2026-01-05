#pragma once
#include <string>
#include "Texture.h"

class Texture2D : public Texture
{
private:
    std::string m_filepath;

    void LoadTexture(bool vertical_flip);

public:
    Texture2D(const std::string& filepath, bool vertical_flip = true);
    ~Texture2D() = default;

    // Prevent copying
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    // Allow moving
    Texture2D(Texture2D&& other) noexcept;
    Texture2D& operator=(Texture2D&& other) noexcept;

    const std::string& GetFilepath() const { return m_filepath; }
};