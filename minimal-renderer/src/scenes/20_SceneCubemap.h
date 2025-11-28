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
        // not the most optimal way but it will work for now
        std::vector<std::unique_ptr<Model>> objects;
        std::vector<std::unique_ptr<Model>> ref_objects;
        
        std::shared_ptr<TextureCubemap> cubemap;
        std::unique_ptr<VertexArray>    skybox_va;
        std::unique_ptr<VertexBuffer>   skybox_vb;
        std::unique_ptr<Material>       skybox_material;

        bool m_use_reflections = false;

    public:
        SceneCubemap(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ConstructScene();
        void ConstructReflectionScene(std::shared_ptr<Texture> in_skybox);

    };
}
