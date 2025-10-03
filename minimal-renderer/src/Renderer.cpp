#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"

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
	ToggleWireframeRender();
	ToggleDepthTest();
	ToggleFaceCulling();

	GLCall(glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, m_background_color.w));
	GLCall(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(Model& obj)
{
    for (auto& mesh : obj.GetMeshes())
    {
        IndexBuffer* ib = mesh->GetIndexBuffer();
        auto& material = mesh->GetMaterial();

        material.Bind();
        material.GetShader().Bind();
        mesh->GetVertexArray()->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
        material.Unbind();
    }
}

void Renderer::ToggleWireframeRender() const
{
	if (m_enable_wireframe)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void Renderer::ToggleFaceCulling() const
{
	if (m_enable_face_culling)
    {
		GLCall(glEnable(GL_CULL_FACE));
    }
    else
    {
        GLCall(glDisable(GL_CULL_FACE));
    }
}

void Renderer::Tick(double current_time)
{
	m_delta_time = current_time - m_last_frame;
	m_last_frame = current_time;
}

void Renderer::ToggleDepthTest() const
{
	if (m_use_depth_buffer)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else
    {
		GLCall(glDisable(GL_DEPTH_TEST));
    }
}

int Renderer::GetMaxVertexAttribs() const
{
	int nrAttr;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
	return nrAttr;
}

Camera& Renderer::GetActiveCamera() const
{
    std::shared_ptr<Camera> camera = state->cameras[state->active_camera];
    return *camera;
}
