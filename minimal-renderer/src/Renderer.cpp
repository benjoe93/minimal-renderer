#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"

#include "Renderer.h"


Renderer::Renderer()
{   
    SetBlending(m_blending);
    SetBlendFunction(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);
    SetBlendEquation(BlendEquation::ADD); // this is the default 

	SetDepthTest(m_depth_buffer);
    SetStencilTest(m_stencil_buffer);

	SetWireframeRender(m_wireframe);

	SetFaceCulling(m_face_culling);
    SetFaceCullingMode(FaceCullMode::BACK);

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

void Renderer::SetBlending(bool enabled)
{
    m_blending = enabled;

    if (enabled)
    {
        GLCall(glEnable(GL_BLEND));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }
}

void Renderer::SetDepthTest(bool enabled)
{
    m_depth_buffer = enabled;

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
    m_face_culling = enabled;

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
    m_stencil_buffer = enabled;

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
    m_wireframe = enabled;

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

void Renderer::SetBlendFunction(BlendFunc src_factor, BlendFunc dst_factor)
{
    GLCall(glBlendFunc(GetBlendFunction(src_factor), GetBlendFunction(dst_factor)));
}

void Renderer::SetBlendEquation(BlendEquation eq)
{
    GLCall(glBlendEquation(GetBlendEquation(eq)));
}

void Renderer::SetFaceCullingMode(FaceCullMode mode)
{
    GLCall(glCullFace(GetCullFaceMode(mode)));
}

void Renderer::SetFrontFace(FrontFace mode)
{
    if (mode == FrontFace::CCW)
    {
        GLCall(glFrontFace(GL_CCW));
    }
    else
    {
        GLCall(glFrontFace(GL_CW));
    }
}

int Renderer::GetMaxVertexAttribs() const
{
	int nrAttr;
	GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
	return nrAttr;
}

GLenum Renderer::GetBlendFunction(BlendFunc id) const
{
    switch (id)
    {
    case ZERO:                      return GL_ZERO;
    case ONE:                       return GL_ONE;
    case SRC_COLOR:                 return GL_SRC_COLOR;
    case ONE_MINUS_SRC_COLOR:       return GL_ONE_MINUS_SRC_COLOR;
    case DST_COLOR:                 return GL_DST_COLOR;
    case ONE_MINUS_DST_COLOR:       return GL_ONE_MINUS_DST_COLOR;
    case SRC_ALPHA:                 return GL_SRC_ALPHA;
    case ONE_MINUS_SRC_ALPHA:       return GL_ONE_MINUS_SRC_ALPHA;
    case DST_ALPHA:                 return GL_DST_ALPHA;
    case ONE_MINUS_DST_ALPHA:       return GL_ONE_MINUS_DST_ALPHA;
    case CONSTANT_COLOR:            return GL_CONSTANT_COLOR;
    case ONE_MINUS_CONSTANT_COLOR:  return GL_ONE_MINUS_CONSTANT_COLOR;
    case CONSTANT_ALPHA:            return GL_CONSTANT_ALPHA;
    case ONE_MINUS_CONSTANT_ALPHA:  return GL_ONE_MINUS_CONSTANT_ALPHA;
    default:                        return GL_ZERO;;
    }
}

GLenum Renderer::GetBlendEquation(BlendEquation id) const
{
    switch (id)
    {
    case ADD:               return GL_FUNC_ADD;
    case SUBTRACT:          return GL_FUNC_SUBTRACT;
    case REVERSE_SUBTRACT:  return GL_FUNC_REVERSE_SUBTRACT;
    case MIN:               return GL_MIN;
    case MAX:               return GL_MAX;
    default:                return GL_FUNC_ADD;
    }
}

GLenum Renderer::GetCullFaceMode(FaceCullMode id) const
{
    switch (id)
    {
    case BACK:              return GL_BACK;
    case FRONT:             return GL_FRONT;
    case FRONT_AND_BACK:    return GL_FRONT_AND_BACK;
    default:                return GL_BACK;
    }
}

Camera& Renderer::GetActiveCamera() const
{
    std::shared_ptr<Camera> camera = state->cameras[state->active_camera];
    return *camera;
}
