#pragma once
#include <memory>
#include "Scene.h"

class Renderer;
class IndexBuffer;
class Texture2D;
class Shader;
class VertexArray;
class VertexBuffer;

namespace scene {
class SceneCube : public Scene
{
private:
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<VertexBuffer> vb;
    std::unique_ptr<IndexBuffer> ib;

    std::unique_ptr<Texture2D> texture_1;
    std::unique_ptr<Texture2D> texture_2;

    std::unique_ptr <Shader> default_shader;

    float offset[3];
    float rotation[3];

public:
    SceneCube(Renderer& in_renderer);
    
    void OnRender() override;
    void OnImGuiRender() override;
};
}