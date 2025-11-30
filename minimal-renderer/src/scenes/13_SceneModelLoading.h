#pragma once
#include <memory>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

struct PointLightData
{
    float position[3];
    float color[3];
};

namespace scene {

    class SceneModelLoading : public Scene
    {
    private:
        std::unique_ptr<DirectionalLight>           directional_light;
        std::vector<std::unique_ptr<PointLight>>    point_lights;
        std::unique_ptr<SpotLight>                  spot_light;

        std::unique_ptr<Model> object;

    public:
        SceneModelLoading();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
