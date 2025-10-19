#pragma once

class Renderer;

namespace scene {

class Scene
{
protected:
    Renderer& m_renderer;
    std::string m_name;

public:
    Scene(Renderer& in_renderer, std::string scene_name) : m_renderer(in_renderer), m_name(scene_name) {}
    virtual ~Scene(){}

    virtual void OnUpdate(double delta_time) {}
    virtual void OnRender() {}
    virtual void OnImGuiRender() {}

    std::string GetSceneName() const { return m_name; }
};
}
