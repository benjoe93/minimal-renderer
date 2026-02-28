#pragma once
#include <memory>
#include "../Scene.h"

class Renderer;
class IndexBuffer;
class Texture2D;
class Material;
class VertexArray;
class VertexBuffer;

class SceneCube : public Scene
{
private:
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;

    Material* material;

    float location[3] = { 0.0f, 0.0f, 0.0f };
    float rotation[3] = { 0.0f, 0.0f, 0.0f };

public:
    SceneCube();

    static std::string StaticName() { return "06_Cube"; }

    void OnRender() override;
    void OnImGuiRender() override;
};
