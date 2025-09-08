#include "Renderer.h"


Renderer::Renderer(const bool DepthTesting, const bool Wireframe, const bool FaceCulling)
	: m_BackgroundColor({ 0.0f, 0.0f, 0.0f, 1.0f }), m_UseDepthBuffer(DepthTesting), m_EnableWireframe(Wireframe), m_EnableFaceCulling(FaceCulling)
{}

Renderer::~Renderer() {}

void Renderer::Clear()
{
	ToggleWireframeRender();
	ToggleDepthTest();
	ToggleFaceCulling();

	GLCall(glClearColor(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, m_BackgroundColor.w));
	GLCall(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}

void Renderer::ToggleWireframeRender() const
{
	if (m_EnableWireframe)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void Renderer::ToggleFaceCulling() const
{
	if (m_EnableFaceCulling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Renderer::ToggleDepthTest() const
{
	if (m_UseDepthBuffer)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

int Renderer::GetMaxVertexAttribs() const
{
	int nrAttr;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
	return nrAttr;
}
