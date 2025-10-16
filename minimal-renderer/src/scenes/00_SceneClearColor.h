#pragma once

#include "Scene.h"

class Renderer;

namespace scene {
class SceneClearColor : public Scene
{
private:
    float m_clear_color[4];

public:
    SceneClearColor(Renderer& in_renderer);
    
    void OnRender() override;
    void OnImGuiRender() override;
};
}