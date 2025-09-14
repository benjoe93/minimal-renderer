#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#include "Renderer.h"


Renderer::Renderer(const bool depth_testing, const bool wireframe, const bool face_culling)
	: m_background_color({ 0.0f, 0.0f, 0.0f, 1.0f }),
	m_use_depth_buffer(depth_testing),
	m_enable_wireframe(wireframe),
	m_enable_face_culling(face_culling),
	m_delta_time(0.0f),
	m_last_frame(0.0f)
{
	// Log OpenGL stats
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Maximum number of vertex attributes supported: " << GetMaxVertexAttribs() << std::endl;

	state = std::make_unique<AppState>();
}

Renderer::~Renderer() {}

void Renderer::Clear()
{
	TogglewireframeRender();
	ToggleDepthTest();
	Toggleface_culling();

	GLCall(glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, m_background_color.w));
	GLCall(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}

void Renderer::TogglewireframeRender() const
{
	if (m_enable_wireframe)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void Renderer::Toggleface_culling() const
{
	if (m_enable_face_culling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Renderer::Tick(double current_time)
{
	m_delta_time = current_time - m_last_frame;
	m_last_frame = current_time;
}

void Renderer::ToggleDepthTest() const
{
	if (m_use_depth_buffer)
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
