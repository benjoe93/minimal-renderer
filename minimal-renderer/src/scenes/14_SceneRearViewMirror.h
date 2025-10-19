#pragma once
#include <memory>
#include "Scene.h"
#include "Model.h"
#include "Framebuffer.h"
#include "RenderBuffer.h"

class Renderer;
class RenderTarget;

namespace scene {

    class SceneRearViewMirror : public Scene
    {
    private:
        std::unique_ptr<Framebuffer> framebuffer;
        std::shared_ptr<RenderBuffer> render_buffer;
        std::shared_ptr<RenderTarget> render_target;

        std::vector<std::unique_ptr<Model>> objects;
        std::unique_ptr<Model> quad;

    public:
        SceneRearViewMirror(Renderer& in_renderer);

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;

        void ConstructScene();
    };
}