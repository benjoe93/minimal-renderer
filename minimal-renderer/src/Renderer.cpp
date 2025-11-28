#include <cstddef>

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
    std::cout << "OpenGL Version: \t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: \t\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "OpenGL Driver Vendor: \t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: \t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Max vertex attribs: \t" << GetMaxVertexAttribs() << std::endl << std::endl;
}

Renderer::~Renderer() {}

void Renderer::Clear(GLbitfield bits) const
{
    GLCall(glClearColor(m_background_color.x, m_background_color.y, m_background_color.z, m_background_color.w));
    GLCall(glClear(bits));
}

void Renderer::Tick(double current_time)
{
    m_delta_time = current_time - m_last_frame;
    m_last_frame = current_time;
}

#pragma region DrawFunctions
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount() , GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, int count, Material& material) const
{
    material.Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void Renderer::Draw(Model& obj)
{
    for (auto& mesh : obj.GetMeshes())
    {
        IndexBuffer* ib = mesh->GetIndexBuffer();
        auto& material = mesh->GetMaterial();

        material.Bind();
        mesh->GetVertexArray()->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr));
        material.Unbind();
    }
}
#pragma endregion

#pragma region RenderSettings
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

void Renderer::SetDepthFunction(TestingFunc function)
{
    GLCall(glDepthFunc(static_cast<unsigned int>(function)));
}

void Renderer::SetStencilFunction(TestingFunc function, int reference_value, unsigned int bit_mask)
{
    GLCall(glStencilFunc(static_cast<unsigned int>(function), reference_value, bit_mask));
}

void Renderer::SetStencilMask(unsigned int bit_mask)
{
    GLCall(glStencilMask(bit_mask));
}

void Renderer::SetStencilOperation(StencilOp stencil_fail, StencilOp depth_fail, StencilOp pass)
{
    GLCall(glStencilOp(static_cast<unsigned int>(stencil_fail), static_cast<unsigned int>(depth_fail), static_cast<unsigned int>(pass)));
}

void Renderer::SetBlendFunction(BlendFunc src_factor, BlendFunc dst_factor)
{
    GLCall(glBlendFunc(static_cast<unsigned int>(src_factor), static_cast<unsigned int>(dst_factor)));
}

void Renderer::SetBlendEquation(BlendEquation eq)
{
    GLCall(glBlendEquation(static_cast<unsigned int>(eq)));
}

void Renderer::SetFaceCullingMode(FaceCullMode mode)
{
    GLCall(glCullFace(static_cast<unsigned int>(mode)));
}

void Renderer::SetFrontFace(FrontFace mode)
{
    GLCall(glFrontFace(static_cast<unsigned int>(mode)));
}
#pragma endregion

int Renderer::GetMaxVertexAttribs() const
{
    int nrAttr;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
    return nrAttr;
}

Camera& Renderer::GetActiveCamera() const
{
    return *state.cameras.at(state.active_camera);
}