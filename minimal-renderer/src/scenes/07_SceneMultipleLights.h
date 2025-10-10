#pragma once
#include <memory>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Scene.h"
#include "Material.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

static struct PointLightData
{
    float position[3];
    float color[3];
};

namespace scene {

    class SceneMultipleLights : public Scene
    {
    private:
        std::unique_ptr<VertexArray>                object_va;
        std::unique_ptr<VertexBuffer>               object_vb;
        std::unique_ptr<IndexBuffer>                object_ib;
        std::vector<std::unique_ptr<Material>>      object_materials;
    
        std::unique_ptr<DirectionalLight>           directional_light;
        std::vector<std::unique_ptr<PointLight>>    point_lights;
        std::unique_ptr<SpotLight>                  spot_light;
    
        float background_color[3];
        float object_color[3];
    
        float light_color[3];
        float dir_light_direction[3];
        std::vector<PointLightData> ptl_data;
    
    public:
        SceneMultipleLights(Renderer& in_renderer);
    
        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}