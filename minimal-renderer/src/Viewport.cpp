#include "Viewport.h"

#include "AppState.h"
#include "Framebuffer.h"
#include "RenderTarget.h"
#include "RenderBuffer.h"
#include "ResourceManager.h"
#include "vendor/imgui/imgui.h"

Viewport::Viewport(int id)
    : m_id(id)
{
    auto& renderer = Renderer::Get();
    m_width = AppState::Get().GetScreenWidth();
    m_height = AppState::Get().GetScreenHeight();

    m_viewport_name = "viewport_" + std::to_string(m_id);

    // Get render target and render buffer
    m_render_target = ResourceManager::Get().GetRenderTarget(m_viewport_name, m_width, m_height, 3);
    m_render_buffer = ResourceManager::Get().GetRenderBuffer(m_viewport_name, m_width, m_height);

    m_framebuffer = ResourceManager::Get().GetFramebuffer(m_viewport_name);
    m_framebuffer->Bind();
    m_framebuffer->AttachRenderTarget(AttachmentTarget::COLOR0, m_render_target);
    m_framebuffer->AttachRenderBuffer(AttachmentTarget::DEPTH_STENCIL, m_render_buffer);

    if (!m_framebuffer->IsComplete()) {
        std::cout << "Framebuffer incomplete after creation!\n";
    }
}

void Viewport::Bind() {
    m_framebuffer->Bind();
    GLCall(glViewport(0, 0, m_width, m_height));
}

void Viewport::Unbind() {
    m_framebuffer->Unbind();
    auto& renderer = Renderer::Get();
    GLCall(glViewport(0, 0, AppState::Get().GetScreenWidth(), AppState::Get().GetScreenHeight()));
}

void Viewport::Resize(int width, int height) {
    if (width <= 0 || height <= 0)
        return;

    if (width == m_width && height == m_height)
        return;

    // Update size
    m_width = width;
    m_height = height;

    m_render_target->Resize(width, height);
    m_render_buffer->Resize(width, height);
}

void Viewport::RenderImGuiWindow() {
    ImGui::Begin("Viewport");

    // Get the available space in the ImGui window
    ImVec2 viewport_size = ImGui::GetContentRegionAvail();

    int new_width  = static_cast<int>(viewport_size.x);
    int new_height = static_cast<int>(viewport_size.y);

    // Check if we need to resize
    if (new_width > 0 && new_height > 0 && (new_width != m_width || new_height != m_height))
    {
        Resize(new_width, new_height);
    }

    // Display the texture
    ImGui::Image(
        (ImTextureID)(intptr_t)m_render_target->GetId(),
        ImVec2(static_cast<float>(m_width), static_cast<float>(m_height)),
        ImVec2(0, 1),   // UV top-left
        ImVec2(1, 0)    // UV bottom-right
    );

    ImGui::End();
}
