#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "TextureCubemap.h"
#include "ResourceManager.h"

#include "20_SceneCubemap.h"

static std::unordered_map<CubeSide, std::string> textures_faces = {
    { CubeSide::FRONT,    "resources/textures/skybox/front.jpg", },
    { CubeSide::BACK,     "resources/textures/skybox/back.jpg", },
    { CubeSide::TOP,      "resources/textures/skybox/top.jpg", },
    { CubeSide::BOTTOM,   "resources/textures/skybox/bottom.jpg", },
    { CubeSide::LEFT,     "resources/textures/skybox/left.jpg", },
    { CubeSide::RIGHT,    "resources/textures/skybox/right.jpg" }
};
static std::vector<float> skybox_verts = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

namespace scene {
    SceneCubemap::SceneCubemap()
        :Scene("Cubemap")
    {
        skybox_va = std::make_unique<VertexArray>();
        skybox_va->Bind();

        skybox_vb = std::make_unique<VertexBuffer>(skybox_verts.data(), skybox_verts.size() * sizeof(float));
        skybox_vb->Bind();

        skybox_va->SetLayout(*skybox_vb, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        TextureCubemap* cubemap = ResourceManager::Get().GetCubemap(textures_faces);

        skybox_material = std::make_unique<Material>("resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.vert", "resources/shaders/03_AdvancedOpenGL/05_Cubemap/cubemap.frag");
        skybox_material->AddTexture("cubemap", cubemap);

        ConstructScene();
        ConstructReflectionScene(cubemap);
        ConstructRefractionScene(cubemap);
    }

    void SceneCubemap::OnUpdate(double delta_time)
    {
        Renderer::Get().SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

        Camera& cam = Renderer::Get().GetActiveCamera();

        glm::mat4 projection, ModelView, MVP;
        projection = glm::perspective(
            glm::radians(cam.GetFov()),
            static_cast<float>(Renderer::Get().GetScreenWidth()) / static_cast<float>(Renderer::Get().GetScreenHeight()),
            Renderer::Get().GetState().near_plane,
            Renderer::Get().GetState().far_plane);

        switch (m_active_mode)
        {
        case 0:
            for (auto& obj : objects)
            {
                ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
                MVP = projection * ModelView;

                for (Material* mat : obj->GetAllMaterials())
                    mat->SetUniform("mvp", MVP);
            }
            break;
        case 1:
            for (auto& obj : reflection_objects)
            {
                ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
                MVP = projection * ModelView;

                for (Material* mat : obj->GetAllMaterials())
                {
                    mat->SetUniform("mvp", MVP);
                    mat->SetUniform("model", obj->GetModelMatrix());
                    mat->SetUniform("camera_position", cam.GetPosition());
                }
            }
            break;
        case 2:
            for (auto& obj : refraction_objects)
            {
                ModelView = cam.GetViewMatrix() * obj->GetModelMatrix();
                MVP = projection * ModelView;

                for (Material* mat : obj->GetAllMaterials())
                {
                    mat->SetUniform("mvp", MVP);
                    mat->SetUniform("model", obj->GetModelMatrix());
                    mat->SetUniform("camera_position", cam.GetPosition());
                }
            }
            break;
        }

        // skybox
        glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix())); // removes translation
        MVP = projection * view;
        skybox_material->SetUniform("mvp", MVP);
    }

    void SceneCubemap::OnRender()
    {
        switch (m_active_mode)
        {
        case 0:
            for (auto& obj : objects)
                Renderer::Get().Draw(*obj);
            break;
        case 1:
            for (auto& obj : reflection_objects)
                Renderer::Get().Draw(*obj);
            break;
        case 2:
            for (auto& obj : refraction_objects)
                Renderer::Get().Draw(*obj);
            break;
        }

        // Optimization: Rendering skybox after the scene is done so fragments covered by the scene doesn't need to be re-rendered.
        Renderer::Get().SetDepthFunction(TestingFunc::LEQUAL);
        Renderer::Get().Draw(*skybox_va, static_cast<int>(skybox_verts.size()), *skybox_material);
        Renderer::Get().SetDepthFunction(TestingFunc::LESS);
    }

    void SceneCubemap::OnImGuiRender()
    {
        ImGui::Begin(m_name.c_str());

        ImGui::Combo("View Mode", &m_active_mode, m_view_modes, 3);

        ImGui::End();
    }

    void SceneCubemap::ConstructScene()
    {
        Texture2D* metal_tex = ResourceManager::Get().GetTexture2D("resources/textures/metal.png", true);
        Texture2D* marble_tex = ResourceManager::Get().GetTexture2D("resources/textures/container.jpg", true);

        Material* object_material = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.vert",
            "resources/shaders/03_AdvancedOpenGL/02_StencilTesting/object.frag"
        );

        auto floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            Transform(
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        floor->SetMaterialSlot(0, object_material);

        for (auto& mesh : floor->GetMeshes()) {
            Material* mat = floor->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", metal_tex);
        }
        objects.push_back(std::move(floor));

        // Box 1
        auto box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(-1.5f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box1->SetMaterialSlot(0, object_material);

        for (auto& mesh : box1->GetMeshes()) {
            Material* mat = box1->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }
        objects.push_back(std::move(box1));

        // Box 2
        auto box2 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box2->SetMaterialSlot(0, object_material);

        for (auto& mesh : box2->GetMeshes()) {
            Material* mat = box2->GetMaterialForMesh(mesh.get());
            if (mat) mat->AddTexture("material.diffuse", marble_tex);
        }
        objects.push_back(std::move(box2));
    }

    void SceneCubemap::ConstructReflectionScene(TextureCubemap* in_skybox)
    {
        Material* reflection_material = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.vert",
            "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.frag"
        );

        auto floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            Transform(
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        floor->SetMaterialSlot(0, reflection_material);

        for (Material* mat : floor->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        reflection_objects.push_back(std::move(floor));

        // Box 1
        auto box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(-1.5f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box1->SetMaterialSlot(0, reflection_material);

        for (Material* mat : box1->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        reflection_objects.push_back(std::move(box1));

        // Box 2
        auto box2 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box2->SetMaterialSlot(0, reflection_material);

        for (Material* mat : box2->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        reflection_objects.push_back(std::move(box2));
    }

    void SceneCubemap::ConstructRefractionScene(TextureCubemap* in_skybox)
    {
        Material* refraction_material = ResourceManager::Get().GetMaterial(
            "resources/shaders/03_AdvancedOpenGL/05_Cubemap/reflection.vert",
            "resources/shaders/03_AdvancedOpenGL/05_Cubemap/refraction.frag"
        );

        auto floor = std::make_unique<Model>(
            "resources/models/plane.fbx",
            Transform(
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(-90.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        floor->SetMaterialSlot(0, refraction_material);

        for (Material* mat : floor->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        refraction_objects.push_back(std::move(floor));

        // Box 1
        auto box1 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(-1.5f, 0.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box1->SetMaterialSlot(0, refraction_material);

        for (Material* mat : box1->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        refraction_objects.push_back(std::move(box1));

        // Box 2
        auto box2 = std::make_unique<Model>(
            "resources/models/box.fbx",
            Transform(
                glm::vec3(1.5f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f)
            )
        );
        box2->SetMaterialSlot(0, refraction_material);

        for (Material* mat : box2->GetAllMaterials())
            mat->AddTexture("cubemap", in_skybox);
        refraction_objects.push_back(std::move(box2));
    }
}