#pragma once
#include <string>

class Renderer;

namespace scene {
    class Scene
    {
    protected:
        std::string m_name;

    public:
        Scene(const std::string &scene_name) : m_name(scene_name) {}
        virtual ~Scene(){}

        virtual void OnUpdate(double delta_time) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}

        std::string GetSceneName() const { return m_name; }
    };
}
