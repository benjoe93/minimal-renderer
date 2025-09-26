#include "Renderer.h"
#include "vendor/imgui/imgui.h"
#include "00_SceneClearColor.h"

namespace scene {
	scene::SceneClearColor::SceneClearColor(Renderer& in_renderer)
		: Scene(in_renderer), 
		m_clear_color { 0.2f, 0.3f, 0.8f, 1.0f }
	{}

	void scene::SceneClearColor::OnRender()
	{
		m_renderer.SetBackgroundColor({ m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3] });
	}

	void scene::SceneClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_clear_color);
	}
}
