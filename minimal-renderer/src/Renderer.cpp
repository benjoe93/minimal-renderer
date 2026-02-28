#include "Material.h"
#include "Model.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Renderer.h"

Renderer::Renderer() {
    SetBlending(m_blending);
    SetBlendFunction(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);
    SetBlendEquation(BlendEquation::ADD); // this is the default 

    SetDepthTest(m_depth_buffer);
    SetStencilTest(m_stencil_buffer);

    SetWireframeRender(m_wireframe);

    SetFaceCulling(m_face_culling);
    SetFaceCullingMode(FaceCullMode::BACK);

    // Log OpenGL stats
#ifndef NDEBUG
    std::cout << "OpenGL Version: \t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: \t\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "OpenGL Driver Vendor: \t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: \t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Max vertex attribs: \t" << GetMaxVertexAttribs() << std::endl << std::endl;
#endif
}

Renderer& Renderer::Get() {
    static auto m_instance = Renderer();
    return m_instance;
}

void Renderer::Clear(GLbitfield bits) const {
    GLCall(glClear(bits));
}

void Renderer::Tick(double current_time) {
    // If m_last_frame hasn't been set yet, initialize it
    if (m_last_frame == 0.0) {
        m_last_frame = current_time;
    }

    double frame_time = current_time - m_last_frame;

    // Cap the delta time to 0.1s (10 FPS) to prevent
    // physics "explosions" after a hang or pause.
    if (frame_time > 0.1) {
        frame_time = 0.1;
    }

    m_delta_time = frame_time;
    m_last_frame = current_time;
}

#pragma region DrawFunctions
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, Material* material) const {
    material->Bind();
    va.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ib.GetCount()), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, int count, Material* material) const {
    material->Bind();
    va.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void Renderer::Draw(Model& obj) {
    for (auto& mesh : obj.GetMeshes()) {
        Material* material = obj.GetMaterialForMesh(mesh.get());

        if (material) {
            material->Bind();
            mesh->GetVertexArray().Bind();
            GLCall(glDrawElements(GL_TRIANGLES, mesh->GetIndexBuffer().GetCount(), GL_UNSIGNED_INT, nullptr));
            material->Unbind();
        }
    }
}
#pragma endregion

#pragma region RenderSettings
void Renderer::SetBlending(bool enabled) {
    m_blending = enabled;

    if (enabled) {
        GLCall(glEnable(GL_BLEND));
    }
    else {
        GLCall(glDisable(GL_BLEND));
    }
}

void Renderer::SetDepthTest(bool enabled) {
    m_depth_buffer = enabled;

    if (enabled) {
        GLCall(glEnable(GL_DEPTH_TEST));
    }
    else {
        GLCall(glDisable(GL_DEPTH_TEST));
    }
}

void Renderer::SetFaceCulling(bool enabled) {
    m_face_culling = enabled;

    if (enabled) {
        GLCall(glEnable(GL_CULL_FACE));
    }
    else {
        GLCall(glDisable(GL_CULL_FACE));
    }
}

void Renderer::SetStencilTest(bool enabled) {
    m_stencil_buffer = enabled;

    if (enabled) {
        GLCall(glEnable(GL_STENCIL_TEST));
    }
    else {
        GLCall(glDisable(GL_STENCIL_TEST));
    }
}

void Renderer::SetWireframeRender(bool enabled) {
    m_wireframe = enabled;

    if (enabled) {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }
}

void Renderer::SetDepthFunction(TestingFunc function) {
    GLCall(glDepthFunc(static_cast<GLenum>(function)));
}

void Renderer::SetStencilFunction(TestingFunc function, int reference_value, unsigned int bit_mask) {
    GLCall(glStencilFunc(static_cast<GLenum>(function), reference_value, bit_mask));
}

void Renderer::SetStencilMask(unsigned int bit_mask) {
    GLCall(glStencilMask(bit_mask));
}

void Renderer::SetStencilOperation(StencilOp stencil_fail, StencilOp depth_fail, StencilOp pass) {
    GLCall(glStencilOp(static_cast<GLenum>(stencil_fail), static_cast<GLenum>(depth_fail), static_cast<GLenum>(pass)));
}

void Renderer::SetBlendFunction(BlendFunc src_factor, BlendFunc dst_factor) {
    GLCall(glBlendFunc(static_cast<GLenum>(src_factor), static_cast<GLenum>(dst_factor)));
}

void Renderer::SetBlendEquation(BlendEquation eq) {
    GLCall(glBlendEquation(static_cast<GLenum>(eq)));
}

void Renderer::SetFaceCullingMode(FaceCullMode mode) {
    GLCall(glCullFace(static_cast<GLenum>(mode)));
}

void Renderer::SetFrontFace(FrontFace mode) {
    GLCall(glFrontFace(static_cast<GLenum>(mode)));
}

void Renderer::SetBackgroundColor(const glm::vec4 &new_color) {
    m_background_color = new_color;
    GLCall(glClearColor(new_color.r, new_color.g, new_color.b, new_color.a));
}
#pragma endregion

GLint Renderer::GetMaxVertexAttribs() const {
    GLint nrAttr;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttr));
    return nrAttr;
}
