#pragma once
#include <memory>
#include "../Scene.h"

class Renderer;
class IndexBuffer;
class Texture2D;
class Shader;
class VertexArray;
class VertexBuffer;

class SceneCubeMultiple : public Scene
{
private:
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;

    Material* material;

public:
    SceneCubeMultiple();

    static std::string StaticName() { return "07_MultipleCubes"; }

    void OnRender() override;
};
