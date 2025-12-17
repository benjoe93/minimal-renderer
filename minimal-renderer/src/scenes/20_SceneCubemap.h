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
        std::vector<std::unique_ptr<Model>> reflection_objects;
        std::vector<std::unique_ptr<Model>> refraction_objects;

        std::unique_ptr<VertexArray>    skybox_va;
        std::unique_ptr<VertexBuffer>   skybox_vb;
        std::unique_ptr<Material>       skybox_material;

        int m_active_mode = 0;
        const char* m_view_modes[3] = { "Normal", "Reflection", "Refraction" };

    public:
        SceneCubemap();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
        void ConstructScene();
        void ConstructReflectionScene(TextureCubemap* in_skybox);
        void ConstructRefractionScene(TextureCubemap* in_skybox);

    };
}
