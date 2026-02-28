#pragma once

#include "../Scene.h"

class Renderer;

class SceneClearColor : public Scene
{
private:
    float m_clear_color[4] = { 0.2f, 0.3f, 0.8f, 1.0f };

public:
    SceneClearColor();

    static std::string StaticName() { return "00_ClearColor"; }
    
    void OnRender() override;
    void OnImGuiRender() override;
};
