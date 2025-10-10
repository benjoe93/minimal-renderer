#pragma once
#include <iostream>

enum TextureType
{
    NONE = 0,
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT,
};

class Texture
{
	private:
		unsigned int m_renderer_id;
		int m_width;
		int m_height;
		int m_nr_channels;

        std::string m_filepath;
        std::string m_sampler_name;
        TextureType m_type;

        GLenum GetFormat() const;
        std::string GetShaderUsage() const;
        void LoadTexture(bool vertical_flip);

	public:
		// connstuctor to read and build the texture
        Texture(const std::string& filepath, TextureType type ,bool vertical_flip = true);
        Texture(const std::string& filepath, std::string sampler_name, bool vertical_flip = true);
        ~Texture();

        // Prevent copying (textures own GPU resources)
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        // Allow moving
        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
        inline std::string GetFilepath() const { return m_filepath; }
        inline std::string GetSamplerName() const { return m_sampler_name; }

        void SetWrappingHorizontal(const GLint new_wrapping);
        void SetWrappingVertical(const GLint new_wrapping);
};