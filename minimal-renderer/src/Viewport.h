#pragma once
#include <memory>
#include "Framebuffer.h"

class RenderBuffer;
class RenderTarget;

class Viewport {
private:
    Framebuffer* m_framebuffer;
    RenderTarget* m_render_target;
    RenderBuffer* m_render_buffer;

    int m_width, m_height, m_id;
    std::string m_viewport_name;


public:
    Viewport(int id = 0);
    ~Viewport() = default;

    // Bind framebuffer for scene rendering
    void Bind();
    // Unbind back to screen
    void Unbind();
    // Recreate framebuffer at new size
    void Resize(int width, int height);
    // Display the viewport in an ImGui window
    void RenderImGuiWindow();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    int GetId() const { return m_id; }
    const std::string& GetViewportName() const { return m_viewport_name; }
};