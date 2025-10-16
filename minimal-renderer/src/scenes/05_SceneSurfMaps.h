#pragma once
#include <memory>
#include "Scene.h"
#include "Material.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
//class Material;

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

    float object_color[3];

    float light_color[3];
    float light_position[3];

public:
    SceneSurfMaps(Renderer& in_renderer);

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
}