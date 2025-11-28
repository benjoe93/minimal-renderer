#pragma once
#include <memory>
#include "Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace scene {

    class SceneBlending : public Scene
    {
    private:
        std::vector<std::unique_ptr<Model>> objects;
        std::vector<std::unique_ptr<Model>> transparent_objects;
        std::vector<glm::vec3> window_loc;
        std::vector<glm::vec3> vegetation_loc;



    public:
        SceneBlending(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}
