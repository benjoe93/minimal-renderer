#pragma once
#include "glad/glad.h"

class Texture
{
public:
	// texture ID
	unsigned int ID;

	// connstuctor to read and build the texture
	Texture(const char* texturePath, GLenum format);

	// texture unit
	void setTextureUnit(const GLenum texture);
};