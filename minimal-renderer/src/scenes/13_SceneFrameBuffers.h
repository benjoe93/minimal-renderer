#pragma once
#include <memory>
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

    class SceneFramebuffer: public Scene
    {
    private:
        std::vector<std::unique_ptr<Model>> objects;
        unsigned int fbo;
        unsigned int texture;
        unsigned int rbo;

        std::unique_ptr<VertexArray>    va;
        std::unique_ptr<VertexBuffer>   vb;
        std::unique_ptr<IndexBuffer>    ib;
        std::unique_ptr<Material>       mat;


    public:
        SceneFramebuffer(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;

        void ConstructScene();
    };
}