#pragma once
#include <memory>
#include "../Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

class SceneDepthTesting : public Scene
{
private:
    std::vector<std::unique_ptr<Model>> objects;

public:
    SceneDepthTesting();

    static std::string StaticName() { return "14_DepthTest"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
