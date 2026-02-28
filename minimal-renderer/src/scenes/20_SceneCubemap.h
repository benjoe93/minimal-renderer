#pragma once
#include <memory>
#include <vector>
#include "../Scene.h"
#include "Model.h"

class Renderer;
class VertexArray;
class VertexBuffer;
class IndexBuffer;
class TextureCubemap;

class SceneCubemap : public Scene
{
private:
    // not the most optimal way but it will work for now
    std::vector<std::unique_ptr<Model>> objects;

    std::unique_ptr<VertexArray>    skybox_va;
    std::unique_ptr<VertexBuffer>   skybox_vb;
    Material* skybox_material;
    TextureCubemap* cubemap;

    int m_active_mode = 0;
    const char* m_view_modes[3] = { "Normal", "Reflection", "Refraction" };

public:
    SceneCubemap();

    static std::string StaticName() { return "20_Cubemap"; }

    void OnUpdate(double delta_time) override;
    void OnRender() override;
    void OnImGuiRender() override;
    void ConstructScene();
    void SetNormalScene();
    void SetReflectionScene();
    void SetRefractionScene();

};
