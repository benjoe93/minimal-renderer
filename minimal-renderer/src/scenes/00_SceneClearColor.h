#pragma once

#include "Scene.h"

class Renderer;

namespace scene {
class SceneClearColor : public Scene
{
private:
    float m_clear_color[4] = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
    SceneClearColor(Renderer& in_renderer);
    
    void OnRender() override;
    void OnImGuiRender() override;
};
}