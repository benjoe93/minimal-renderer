#pragma once
#include <memory>
#include <vector>
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Scene.h"
#include "Material.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {
    class _SceneEmpty : public Scene
    {
    private:
        std::vector<Model*> m_objects;

    public:
        _SceneEmpty();
        ~_SceneEmpty();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ConstructScene();
    };
}