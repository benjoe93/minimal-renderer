#pragma once
#include <memory>
#include "../Scene.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class Material;

class SceneTransform : public Scene
{
private:
    Material* material;

    std::unique_ptr<VertexArray>  vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer>  ebo;

    float location[2] = {0.0f, 0.0f};
    float angle = 45.0f;

public:
    SceneTransform();

    static std::string StaticName() { return "04_Transform"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
