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

#include "SceneColor.h"

namespace scene {

	SceneColor::SceneColor(Renderer& in_renderer)
		:Scene(in_renderer),
		light_color		{ 1.0f, 1.0f, 1.0f },
		object_color	{ 1.0f, 0.5f, 0.31f },
		light_position	{ 1.2f, 1.0f, 2.0f }
	{
		/* QUAD DEFINITION */
		float vertices[] = {
			// positions
			 0.5f,  0.5f,  0.5f,	// 0: front top right
			 0.5f, -0.5f,  0.5f,	// 1: front bottom right
			-0.5f, -0.5f,  0.5f,	// 2: front bottom left
			-0.5f,  0.5f,  0.5f,	// 3: front top left

			 0.5f,  0.5f, -0.5f,	// 4: back top right
			 0.5f, -0.5f, -0.5f,	// 5: back bottom right
			-0.5f, -0.5f, -0.5f,	// 6: back bottom left
			-0.5f,  0.5f, -0.5f		// 7: back top left
		};

		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3,
			4, 7, 6,
			4, 6, 5,
			0, 3, 7,
			0, 7, 4,
			1, 5, 6,
			1, 6, 2,
			0, 4, 5,
			0, 5, 1,
			3, 2, 6,
			3, 6, 7
		};
		unsigned int size = 8 * 3 * sizeof(float);

		// Object setup
		object_va = std::make_unique<VertexArray>();
		object_va->Bind();

		object_vb = std::make_unique<VertexBuffer>(vertices, size);
		object_vb->Bind();
		object_ib = std::make_unique<IndexBuffer>(indices, 36);
		object_ib->Bind();

		object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		object_shader = std::make_unique<Shader>("resources/shaders/Lighting/object.vert", "resources/shaders/Lighting/object.frag");
		object_shader->Bind();
		object_shader->SetVec3("inLightColor", light_color[0], light_color[1], light_color[2]);
		object_shader->SetVec3("inObjectColor", object_color[0], object_color[1], object_color[2]);

		object_va->Unbind();
		object_vb->Unbind();
		object_ib->Unbind();
		object_shader->Unbind();

		// Light setup
		light_va = std::make_unique<VertexArray>();
		light_va->Bind();
		
		light_vb = std::make_unique<VertexBuffer>(vertices, size);
		light_vb->Bind();
		light_ib = std::make_unique<IndexBuffer>(indices, 36);
		light_ib->Bind();
		
		light_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		
		light_shader = std::make_unique<Shader>("resources/shaders/Lighting/light.vert", "resources/shaders/Lighting/light.frag");
		light_shader->Bind();
		light_shader->SetVec3("inLightColor", light_color[0], light_color[1], light_color[2]);

		light_va->Unbind();
		light_vb->Unbind();
		light_ib->Unbind();
		light_shader->Unbind();
	}

	void SceneColor::OnRender()
	{
		// Object rendering
		glm::mat4 projection = glm::perspective(glm::radians(m_renderer.state->active_camera->GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);

		object_shader->Bind();
		object_shader->SetMat4("model", model);
		object_shader->SetMat4("view", m_renderer.state->active_camera->GetCurrentView());
		object_shader->SetMat4("projection", projection);

		object_shader->SetVec3("inLightColor", light_color[0], light_color[1], light_color[2]);
		object_shader->SetVec3("inObjectColor", object_color[0], object_color[1], object_color[2]);

		m_renderer.Draw(*object_va, *object_ib, *object_shader);

		// Light rendering
		light_shader->Bind();
		glm::mat4 light_transform = glm::mat4(1.0f);
		light_transform = glm::translate(light_transform, glm::vec3(light_position[0], light_position[1], light_position[2]));
		light_shader->SetMat4("model", light_transform);
		light_shader->SetMat4("view", m_renderer.state->active_camera->GetCurrentView());
		light_shader->SetMat4("projection", projection);

		light_shader->SetVec3("inLightColor", light_color[0], light_color[1], light_color[2]);

		m_renderer.Draw(*light_va, *light_ib, *light_shader);
	}

	void SceneColor::OnImGuiRender()
	{
		ImGui::Begin("Colors");
		ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);
		
		ImGui::Separator();

		ImGui::ColorEdit3("Light Color", light_color);
		ImGui::ColorEdit3("Object Color", object_color);
		ImGui::End();
	}
}