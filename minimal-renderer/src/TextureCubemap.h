#pragma once
#include <string>
#include <unordered_map>
#include "Renderer.h"
#include "Texture.h"

enum CubeSide
{
    FRONT = 0,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};
enum CubePosition
{
    POSITIVE_X  = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    POSITIVE_Y  = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    POSITIVE_Z  = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    NEGATIVE_X  = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    NEGATIVE_Y  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    NEGATIVE_Z  = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

class TextureCubemap : public Texture
{
    private:
        std::unordered_map<CubeSide, std::string> m_textures_paths;

        void LoadTextureForSide(CubeSide side, CubePosition position);

    public:
        TextureCubemap(std::unordered_map<CubeSide, std::string> side_source = {});

        void Bind(unsigned int slot = 0) const override;
        void Unbind() const override;
};
