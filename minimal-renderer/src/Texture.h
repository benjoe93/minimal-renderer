#pragma once
#include "glad/glad.h"

class Texture
{
private:
	unsigned int m_RendererID;
	int m_Width;
	int m_Height;
	int m_NrChannels;
public:
	// connstuctor to read and build the texture
	Texture(const char* texturePath, GLenum format, bool vertical_flip = true);
	~Texture();

	// texture unit
	void setTextureUnit(const GLenum texture);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

};