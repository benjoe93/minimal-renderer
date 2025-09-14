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

#include "SceneCubeMultiple.h"

namespace scene {
	scene::SceneCubeMultiple::SceneCubeMultiple(Renderer& in_renderer)
		:Scene(in_renderer)
	{
		float vertices[] = {
			// positions            // uvs        // colors           
			-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,    0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,   1.0f, 0.0f, 0.0f
		};
		unsigned int indices[] = {
			 0,  1,  2,
			 3,  4,  5,
			 6,  7,  8,
			 9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		};

		// Crate VAO, VBO, EBO
		va = std::make_unique<VertexArray>();
		va->Bind();

		unsigned int size = 36 * 8 * sizeof(float);
		vb = std::make_unique<VertexBuffer>(vertices, size);
		vb->Bind();
		ib = std::make_unique<IndexBuffer>(indices, 36);

		va->SetLayout(*vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // vertex position
		va->SetLayout(*vb, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // uv coords
		va->SetLayout(*vb, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));  // vertex color

		vb->Unbind();
		va->Unbind();

		// Create textures
		texture_1 = std::make_unique<Texture>("resources/textures/container.jpg", GL_RGB);
		texture_2 = std::make_unique<Texture>("resources/textures/awesomeface.png", GL_RGBA);

		texture_1->Bind();
		texture_2->Bind(1);

		// Create shaders
		default_shader = std::make_unique<Shader>("resources/shaders/GettingStarted/default.vert", "resources/shaders/GettingStarted/default.frag");

		// Pass textures to shader
		default_shader->Bind();
		default_shader->SetInt("texture_1", 0);
		default_shader->SetInt("texture_2", 1);

		// Set scene background color
		m_renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	}

	void scene::SceneCubeMultiple::OnRender()
	{
		// Set scene background color
		m_renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });

		// position vectors to render multiple cubes
		glm::vec3 cube_positions[] = {
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
		
		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(m_renderer.state->active_camera->GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

		// Model matrix
		for (unsigned char i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube_positions[i]);
			// set rotation
			float rotation = i * 20.0f;
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.3f, 0.5f));

			default_shader->SetMat4("model", model);
			default_shader->SetMat4("view", m_renderer.state->active_camera->GetCurrentView());
			default_shader->SetMat4("projection", projection);

			// draw first triangle
			m_renderer.Draw(*va, *ib , *default_shader);
		}
	}
}
