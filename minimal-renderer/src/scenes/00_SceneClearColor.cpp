#include "Renderer.h"
#include "vendor/imgui/imgui.h"
#include "00_SceneClearColor.h"

#include "SceneRegistry.h"

SceneClearColor::SceneClearColor()
    : Scene(StaticName())
{}

void SceneClearColor::OnRender()
{
    Renderer::Get().SetBackgroundColor({ m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3] });
}

void SceneClearColor::OnImGuiRender()
{
    ImGui::Begin(m_name.c_str());
    ImGui::ColorEdit4("Clear Color", m_clear_color);
    ImGui::End();
}

REGISTER_SCENE(SceneClearColor);