#pragma once

class Texture
{
	private:
		unsigned int m_renderer_id;
		int m_width;
		int m_height;
		int m_num_channels;

	public:
		// connstuctor to read and build the texture
		Texture(const char* texture_path, unsigned int format, bool vertical_flip = true);
		~Texture();

		// texture unit
		void setTextureUnit(const unsigned int texture) const;

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
};