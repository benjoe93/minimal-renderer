#pragma once
#include <memory>
#include <vector>
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class TextureCubemap;

namespace scene {
    class SceneCubemap : public Scene
    {
    private:
        std::vector<std::unique_ptr<Model>> objects;
        
        std::shared_ptr<TextureCubemap> cubemap;
        std::unique_ptr<VertexArray>    skybox_va;
        std::unique_ptr<VertexBuffer>   skybox_vb;
        std::unique_ptr<Material>       skybox_material;

    public:
        SceneCubemap(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ConstructScene();
    };
}
