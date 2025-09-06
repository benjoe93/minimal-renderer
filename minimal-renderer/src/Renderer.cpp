#include "Renderer.h"


Renderer::Renderer() : m_BackgroundColor({ 0.0f, 0.0f, 0.0f, 1.0f }) {} 

Renderer::~Renderer() {}

void Renderer::Clear() const
{
	GLCall(glClearColor(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, m_BackgroundColor.w));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}

void Renderer::ToggleWireframeRender(bool IsWireframe)
{
	if (IsWireframe)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

int Renderer::GetMaxVertexAttribs() const
{
	int nrAttr;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
	return nrAttr;
}
