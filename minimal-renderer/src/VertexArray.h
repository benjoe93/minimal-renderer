#pragma once
#include <glad/glad.h>
#include "VertexBuffer.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void SetLayout(const VertexBuffer& vbo, GLint layout, GLint size, GLenum type, GLboolean normalize, GLsizei stride, const void* offset);

	void Bind() const; 
	void Unbind() const;
};
