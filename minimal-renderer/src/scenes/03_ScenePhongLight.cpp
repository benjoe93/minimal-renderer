#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vendor/imgui/imgui.h"

#include "Renderer.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "03_ScenePhongLight.h"

namespace scene {

	ScenePhongLight::ScenePhongLight(Renderer& in_renderer)
		:Scene(in_renderer),
		light_color		{ 1.0f, 1.0f, 1.0f },
		object_color	{ 1.0f, 0.5f, 0.31f },
		light_position	{ 1.2f, 1.0f, 2.0f }
	{
		/* QUAD DEFINITION */
		float vertices[] = {
			// Positions			Normal
			// Front face (z = -0.5)
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
			// Back face (z = 0.5)
			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,
			// Left face (x = -0.5)
			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
			// Right face (x = 0.5)
			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
			// Bottom face (y = -0.5)
			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
			// Top face (y = 0.5)
			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f
		};
		unsigned int indices[] = {
			// Front face
			 0,  1,  2,
			 3,  4,  5,
			// Back face
			 6,  7,  8,
			 9, 10, 11,
			// Left face
			12, 13, 14,
			15, 16, 17,
			// Right face
			18, 19, 20,
			21, 22, 23,
			// Bottom face
			24, 25, 26,
			27, 28, 29,
			// Top face
			30, 31, 32,
			33, 34, 35
		};

		size_t element_size = 36;
		size_t buffer_size = element_size * 6 * sizeof(float);

		// Object setup
		object_va = std::make_unique<VertexArray>();
		object_va->Bind();

		object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
		object_vb->Bind();
		object_ib = std::make_unique<IndexBuffer>(indices, element_size);
		object_ib->Bind();

		object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		object_shader = std::make_unique<Shader>("resources/shaders/01_Lighting/00_PhongLighting/object.vert", "resources/shaders/01_Lighting/00_PhongLighting/object.frag");

		object_va->Unbind();
		object_vb->Unbind();
		object_ib->Unbind();
		object_shader->Unbind();

		// Light setup
		light_va = std::make_unique<VertexArray>();
		light_va->Bind();

		light_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
		light_vb->Bind();
		light_ib = std::make_unique<IndexBuffer>(indices, element_size);
		light_ib->Bind();

		light_va->SetLayout(*light_vb, 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		//light_va->SetLayout(*light_vb, 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));

		light_shader = std::make_unique<Shader>("resources/shaders/01_Lighting/00_PhongLighting/light.vert", "resources/shaders/01_Lighting/00_PhongLighting/light.frag");
		light_shader->Bind();

		light_va->Unbind();
		light_vb->Unbind();
		light_ib->Unbind();
		light_shader->Unbind();
	}

	void ScenePhongLight::OnRender()
	{
		Camera* cam = m_renderer.state->active_camera;
		glm::vec3 cam_pos = cam->GetPosition();

		// Object rendering
		glm::mat4 projection = glm::perspective(glm::radians(cam->GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 mvp_matrix = projection * cam->GetCurrentView() * model;

		object_shader->Bind();
		object_shader->SetMat4("model", model);
		object_shader->SetMat4("view", cam->GetCurrentView());
		object_shader->SetMat4("projection", projection);

		object_shader->SetVec3("u_objectColor", object_color[0], object_color[1], object_color[2]);
		object_shader->SetVec3("u_viewPos", cam_pos[0], cam_pos[1], cam_pos[2]);

		object_shader->SetVec3("u_lightColor", light_color[0], light_color[1], light_color[2]);
		object_shader->SetVec3("u_lightPos", light_position[0], light_position[1], light_position[2]);

		m_renderer.Draw(*object_va, *object_ib, *object_shader);

		// Light rendering
		light_shader->Bind();
		glm::mat4 light_transform = glm::mat4(1.0f);
		light_transform = glm::translate(light_transform, glm::vec3(light_position[0], light_position[1], light_position[2]));
		light_shader->SetMat4("model", light_transform);
		light_shader->SetMat4("view", cam->GetCurrentView());
		light_shader->SetMat4("projection", projection);

		light_shader->SetVec3("u_lightColor", light_color[0], light_color[1], light_color[2]);

		m_renderer.Draw(*light_va, *light_ib, *light_shader);
	}

	void ScenePhongLight::OnImGuiRender()
	{
		ImGui::Begin("Colors");
		ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);

		ImGui::Separator();

		ImGui::ColorEdit3("Light Color", light_color);
		ImGui::ColorEdit3("Object Color", object_color);
		ImGui::End();
	}
}