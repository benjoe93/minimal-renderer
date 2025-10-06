#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"

#include "Renderer.h"


Renderer::Renderer(const bool depth_testing, const bool stencil_testing, const bool wireframe, const bool face_culling)
	: m_background_color({ 0.0f, 0.0f, 0.0f, 1.0f }),
	m_use_depth_buffer(depth_testing),
    m_use_stencil_buffer(stencil_testing),
	m_enable_wireframe(wireframe),
	m_enable_face_culling(face_culling),
	m_delta_time(0.0f),
	m_last_frame(0.0f)
{
	SetDepthTest(depth_testing);
    SetStencilTest(stencil_testing);
	SetWireframeRender(wireframe);
	SetFaceCulling(face_culling);

	// Log OpenGL stats
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Maximum number of vertex attributes supported: " << GetMaxVertexAttribs() << std::endl;

	state = std::make_unique<AppState>();
}

Renderer::~Renderer() {}

void Renderer::Clear() const
{
	GLCall(glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, m_background_color.w));
	GLCall(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
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

void Renderer::Tick(double current_time)
{
	m_delta_time = current_time - m_last_frame;
	m_last_frame = current_time;
}

void Renderer::SetDepthTest(bool enabled)
{
    m_use_depth_buffer = enabled;

	if (enabled)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else
    {
		GLCall(glDisable(GL_DEPTH_TEST));
    }
}

void Renderer::SetFaceCulling(bool enabled)
{
    m_enable_face_culling = enabled;

	if (enabled)
    {
		GLCall(glEnable(GL_CULL_FACE));
    }
    else
    {
        GLCall(glDisable(GL_CULL_FACE));
    }
}

void Renderer::SetStencilTest(bool enabled)
{
    m_use_stencil_buffer = enabled;

    if (enabled)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
    }
    else
    {
        GLCall(glDisable(GL_STENCIL_TEST));
    }
}

void Renderer::SetWireframeRender(bool enabled)
{
    m_enable_wireframe = enabled;

	if (enabled)
	{
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	}
}

void Renderer::SetDepthFunction(GLenum function)
{
   GLCall(glDepthFunc(function));
}

void Renderer::SetStencilFunction(GLenum function, GLint reference_value, GLuint bit_mask)
{
    GLCall(glStencilFunc(function, reference_value, bit_mask));
}

void Renderer::SetStencilMask(GLuint bit_mask)
{
    GLCall(glStencilMask(bit_mask));
}

void Renderer::SetStencilOperation(GLenum stencil_fail, GLenum depth_fail, GLenum pass)
{
    GLCall(glStencilOp(stencil_fail, depth_fail, pass));
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
