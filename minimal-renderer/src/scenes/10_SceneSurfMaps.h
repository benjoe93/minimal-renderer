#pragma once
#include <memory>
#include "Scene.h"
#include "Material.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {
class SceneSurfMaps: public Scene
{
private:
    std::unique_ptr<VertexArray>    object_va;
    std::unique_ptr<VertexBuffer>    object_vb;
    std::unique_ptr<IndexBuffer>    object_ib;
    std::unique_ptr<Material>        object_material;

    std::unique_ptr<VertexArray>    light_va;
    std::unique_ptr<VertexBuffer>    light_vb;
    std::unique_ptr<IndexBuffer>    light_ib;
    std::unique_ptr<Material>        light_material;

    float object_color[3]   = { 1.0f, 0.5f, 0.31f };

    float light_color[3]    = { 1.0f, 1.0f, 1.0f };
    float light_position[3] = { 1.2f, 1.0f, 2.0f };

public:
    SceneSurfMaps(Renderer& in_renderer);

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
}
