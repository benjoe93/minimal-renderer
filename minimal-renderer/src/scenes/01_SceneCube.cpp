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

#include "01_SceneCube.h"

namespace scene {
	scene::SceneCube::SceneCube(Renderer& in_renderer)
		:Scene(in_renderer),
		offset{ 0.0f, 0.0f, 0.0f },
		rotation{ 0.0f, 0.0f, 0.0f }
	{
		/* QUAD DEFINITION */
		float vertices[] = {
			// positions          // uvs        // colors
			 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   // 0: front top right
			 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   // 1: front bottom right
			-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   // 2: front bottom left
			-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 0.0f,   // 3: front top left

			 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f, 0.0f, 1.0f,   // 4: back top right
			 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   // 5: back bottom right
			-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f, 0.0f, 1.0f,   // 6: back bottom left
			-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f, 1.0f, 1.0f    // 7: back top left
		};

		// Index array: defines 12 triangles (2 per face) for 6 cube faces
		// Using counter-clockwise winding order
		unsigned int indices[] = {
		    // Front face (z = +0.5)
		    0, 1, 2,
		    0, 2, 3,
		
		    // Back face (z = -0.5)
		    4, 7, 6,
		    4, 6, 5,
		
		    // Left face (x = -0.5)  
		    0, 3, 7,
		    0, 7, 4,
		
		    // Right face (x = +0.5)
		    1, 5, 6,
		    1, 6, 2,
		
		    // Bottom face (y = -0.5)
		    0, 4, 5,
		    0, 5, 1,
		
		    // Top face (y = +0.5)
		    3, 2, 6,
		    3, 6, 7
		};

		// Crate VAO, VBO, EBO
		va = std::make_unique<VertexArray>();
		va->Bind();

		unsigned int size = 8 * 8 * sizeof(float);
		vb = std::make_unique<VertexBuffer>(vertices, size);
		vb->Bind();
		ib = std::make_unique<IndexBuffer>(indices, 36);

		va->SetLayout(*vb, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);                    // vertex position
		va->SetLayout(*vb, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // uv coords
		va->SetLayout(*vb, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));  // vertex color

		vb->Unbind();
		va->Unbind();

		// Create textures
		texture_1 = std::make_unique<Texture>("resources/textures/container.jpg", TextureType::NONE, true);
		texture_2 = std::make_unique<Texture>("resources/textures/awesomeface.png", TextureType::NONE, true);

		texture_1->Bind();
		texture_2->Bind(1);

		// Create shaders
		default_shader = std::make_unique<Shader>("resources/shaders/00_GettingStarted/default.vert", "resources/shaders/00_GettingStarted/default.frag");

		// Pass textures to shader
		default_shader->Bind();
		default_shader->SetInt("texture_1", 0);
		default_shader->SetInt("texture_2", 1);

		// Set scene background color
		m_renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });
	}

	void scene::SceneCube::OnRender()
	{
		// Set scene background color
		m_renderer.SetBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f });

		// Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(m_renderer.GetActiveCamera().GetFov()), static_cast<float>(m_renderer.state->scr_width) / static_cast<float>(m_renderer.state->scr_height), 0.1f, 100.0f);

		// Model matrix
		glm::mat4 model = glm::mat4(1.0f);
		// transform with user input
		model = glm::translate(model, glm::vec3(offset[0], offset[1], offset[2]));
		model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

		default_shader->SetMat4("model", model);
		default_shader->SetMat4("view", m_renderer.GetActiveCamera().GetViewMatrix());
		default_shader->SetMat4("projection", projection);

		// draw first triangle
		m_renderer.Draw(*va, *ib , *default_shader);
	}

	void scene::SceneCube::OnImGuiRender()
	{
		ImGui::Begin("Cube transform");

		ImGui::Text("Translate");
		ImGui::SameLine();
		ImGui::DragFloat3("##offset", offset, 0.01f);

		ImGui::Text("Rotation ");
		ImGui::SameLine();
		ImGui::DragFloat3("##rotation", rotation, 0.1f);
		
		ImGui::End();
	}
}
