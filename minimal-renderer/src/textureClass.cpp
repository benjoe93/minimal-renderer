#include "textureClass.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* texturePath, GLenum fomat)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// set texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, fomat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << texturePath << std::endl;
	}
	stbi_image_free(data);
}

void Texture::setTextureUnit(GLenum texture)
{
	glActiveTexture(texture);
	glBindTexture(GL_TEXTURE_2D, ID);
}
