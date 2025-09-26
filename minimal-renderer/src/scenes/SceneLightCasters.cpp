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
#include "Material.h"

#include "SceneLightCasters.h"

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

namespace scene {

	SceneLightCasters::SceneLightCasters(Renderer& in_renderer)
		:Scene(in_renderer),
		light_color		{ 1.0f, 1.0f, 1.0f },
		object_color	{ 1.0f, 0.5f, 0.31f },
		light_position	{ 1.2f, 1.0f, 2.0f }
	{
		/* QUAD DEFINITION */
		float vertices[] = {
			// Positions			// Normal				// uv
			// Front face (z = -0.5)
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,	0.0f, 0.0f,
			// Back face (z = 0.5)
			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
			// Left face (x = -0.5)
			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			// Right face (x = 0.5)
			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
			// Bottom face (y = -0.5)
			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,	0.0f, 1.0f,
			// Top face (y = 0.5)
			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f
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
		size_t buffer_size = element_size * 8 * sizeof(float);

		// Object setup
		object_va = std::make_unique<VertexArray>();
		object_va->Bind();

		object_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
		object_vb->Bind();
		object_ib = std::make_unique<IndexBuffer>(indices, element_size);
		object_ib->Bind();

		object_va->SetLayout(*object_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		object_va->SetLayout(*object_vb, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		object_va->SetLayout(*object_vb, 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		for (int i = 0; i < 10; i++)
		{
			object_materials.push_back(std::make_unique<Material>("resources/shaders/Lighting/LightCasters/object.vert", "resources/shaders/Lighting/LightCasters/object.frag"));

			// Bind Textures
			object_materials[i]->AddTexture("material.diffuse", "resources/textures/container2.png", true);
			object_materials[i]->AddTexture("material.specular", "resources/textures/container2_specular.png", true);
		}

		object_va->Unbind();
		object_vb->Unbind();
		object_ib->Unbind();

		// Light setup
		light_va = std::make_unique<VertexArray>();
		light_va->Bind();

		light_vb = std::make_unique<VertexBuffer>(vertices, buffer_size);
		light_vb->Bind();
		light_ib = std::make_unique<IndexBuffer>(indices, element_size);
		light_ib->Bind();

		light_va->SetLayout(*light_vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

		light_material = std::make_unique<Material>("resources/shaders/Lighting/LightCasters/light.vert", "resources/shaders/Lighting/LightCasters/light.frag");

		light_va->Unbind();
		light_vb->Unbind();
		light_ib->Unbind();
	}

	void SceneLightCasters::OnUpdate(double delta_time)
	{

		Camera* cam = m_renderer.state->active_camera;
		glm::vec3 cam_pos = cam->GetPosition();

		glm::mat4 projection, model, ModelView, MVP;
		projection = glm::perspective(glm::radians(cam->GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

		// Object
		/* Directional light */
		/*
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			ModelView = cam->GetCurrentView() * model;
			MVP = projection * ModelView;

			object_materials[i]->SetUniformMat4("model", model);
			object_materials[i]->SetUniformMat4("mvp", MVP);

			object_materials[i]->SetUniformFloat("material.shininess", 32.f);

			object_materials[i]->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			object_materials[i]->SetUniformVec3("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
			object_materials[i]->SetUniformVec3("light.direction", glm::vec3(-light_position[0], -light_position[1], -light_position[2])); // Directon is need to be flipped to get the direction from the fragment to the light
			object_materials[i]->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			object_materials[i]->SetUniformVec3("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
			object_materials[i]->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			object_materials[i]->SetUniformFloat("light.constant", 1.0f);
			object_materials[i]->SetUniformFloat("light.linear", 0.09f);
			object_materials[i]->SetUniformFloat("light.quadratic", 0.032f);

		}
		*/

		/* Point light */
		/*
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			ModelView = cam->GetCurrentView() * model;
			MVP = projection * ModelView;

			object_materials[i]->SetUniformMat4("model", model);
			object_materials[i]->SetUniformMat4("mvp", MVP);

			object_materials[i]->SetUniformFloat("material.shininess", 32.f);

			object_materials[i]->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			object_materials[i]->SetUniformVec3("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));
			object_materials[i]->SetUniformVec3("light.position", glm::vec3(light_position[0], light_position[1], light_position[2]));
			object_materials[i]->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			object_materials[i]->SetUniformVec3("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
			object_materials[i]->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			object_materials[i]->SetUniformFloat("light.constant", 1.0f);
			object_materials[i]->SetUniformFloat("light.linear", 0.09f);
			object_materials[i]->SetUniformFloat("light.quadratic", 0.032f);

		}
		*/

		/* Spotlight */
		for (int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			ModelView = cam->GetCurrentView() * model;
			MVP = projection * ModelView;

			object_materials[i]->SetUniformMat4("model", model);
			object_materials[i]->SetUniformMat4("mvp", MVP);

			object_materials[i]->SetUniformFloat("material.shininess", 32.f);

			object_materials[i]->SetUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			object_materials[i]->SetUniformVec3("u_viewPos", glm::vec3(cam_pos[0], cam_pos[1], cam_pos[2]));

			object_materials[i]->SetUniformVec3("light.position", cam_pos);
			object_materials[i]->SetUniformVec3("light.direction", cam->GetDirection());
			object_materials[i]->SetUniformFloat("light.cut_off", glm::cos(glm::radians(12.5f))); // sending cosine for perf reasons | cos(light_angle) == dot(light_dir, spot_dir)
			object_materials[i]->SetUniformFloat("light.outer_cut_off", glm::cos(glm::radians(17.5f))); // sending cosine for perf reasons | cos(light_angle) == dot(light_dir, spot_dir)

			object_materials[i]->SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			object_materials[i]->SetUniformVec3("light.diffuse", glm::vec3(light_color[0], light_color[1], light_color[2]));
			object_materials[i]->SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			object_materials[i]->SetUniformFloat("light.constant", 1.0f);
			object_materials[i]->SetUniformFloat("light.linear", 0.09f);
			object_materials[i]->SetUniformFloat("light.quadratic", 0.032f);

		}

		// Light
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(light_position[0], light_position[1], light_position[2]));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		ModelView = cam->GetCurrentView() * model;
		MVP = projection * ModelView;
		
		light_material->SetUniformMat4("mvp", MVP);

		light_material->SetUniformVec3("u_lightColor", glm::vec3(light_color[0], light_color[1], light_color[2]));
	}

	void SceneLightCasters::OnRender()
	{
		// Object rendering
		for (int i = 0; i < 10; i++)
		{
			object_materials[i]->Bind();
			m_renderer.Draw(*object_va, *object_ib, *(object_materials[i]->GetShader()));
			object_materials[i]->Unbind();
		}

		// Light rendering
		light_material->Bind();
		m_renderer.Draw(*light_va, *light_ib, *(light_material->GetShader()));
		light_material->Unbind();
	}

	void SceneLightCasters::OnImGuiRender()
	{
		ImGui::Begin("Colors");
		ImGui::SliderFloat3("Light Position", light_position, -10.f, 10.f);

		ImGui::Separator();

		ImGui::ColorEdit3("Light Color", light_color);
		ImGui::ColorEdit3("Object Color", object_color);
		ImGui::End();
	}
}