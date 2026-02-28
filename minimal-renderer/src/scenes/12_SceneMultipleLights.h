#pragma once
#include <memory>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "../Scene.h"
#include "Material.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

struct LightData
{
    float position[3];
    float color[3];
};

class SceneMultipleLights : public Scene
{
private:
    std::unique_ptr<VertexArray>                object_va;
    std::unique_ptr<VertexBuffer>               object_vb;
    std::unique_ptr<IndexBuffer>                object_ib;
    std::vector<Material*> object_materials;

    std::unique_ptr<DirectionalLight>           directional_light;
    std::vector<std::unique_ptr<PointLight>>    point_lights;
    std::unique_ptr<SpotLight>                  spot_light;

    float background_color[3] = { 0.18f,  0.23f, 0.24f };
    float object_color[3] = { 1.0f,  0.5f, 0.31f };

    float light_color[3] = { 1.0f,  1.0f, 1.0f };
    float dir_light_direction[3] = { 0.0f, -1.0f, 0.0f };
    std::vector<LightData> ptl_data;

public:
    SceneMultipleLights();

    static std::string StaticName() { return "12_MulipleLights"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
};
