#pragma once
#include <memory>
#include "../Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

class SceneFaceCulling : public Scene
{
private:
    std::vector<std::unique_ptr<Model>> objects;

public:
    SceneFaceCulling();

    static std::string StaticName() { return "17_FaceCulling"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
