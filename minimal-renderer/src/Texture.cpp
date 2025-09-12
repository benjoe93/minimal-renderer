#include <iostream>

#include "vendor/stb_image/stb_image.h"

#include "Renderer.h"
#include "Texture.h"

Texture::Texture(const char* texture_path, unsigned int fomat, bool vertical_flip)
{
	// sets the image import to be upside down
	// this is neceserry due to how stb and opengl handles coordinates
	stbi_set_flip_vertically_on_load(vertical_flip);

	GLCall(glGenTextures(1, &m_renderer_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

	// set texture wrapping/filtering options
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// load and generate texture
	unsigned char* data = stbi_load(texture_path, &m_width, &m_height, &m_num_channels, 0);
	if (data)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, fomat, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		std::cout << "Failed to load texture: " << texture_path << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
}

void Texture::setTextureUnit(unsigned int texture) const
{
	GLCall(glActiveTexture(texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
