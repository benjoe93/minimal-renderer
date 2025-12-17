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
        RenderBuffer* render_buffer;
        RenderTarget* render_target;

        std::vector<std::unique_ptr<Model>> objects;
        std::unique_ptr<Model> quad;

    public:
        SceneRearViewMirror();
        ~SceneRearViewMirror();

        void OnUpdate(double delta_time) override;
        void OnRender() override;
        void OnImGuiRender() override;

        void ConstructScene();
    };
}