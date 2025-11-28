#pragma once
#include <memory>
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {

    class SceneStencilTesting : public Scene
    {
    private:
        std::vector<std::unique_ptr<Model>> objects;
        std::vector<std::unique_ptr<Model>> outline_objects;


    public:
        SceneStencilTesting(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
