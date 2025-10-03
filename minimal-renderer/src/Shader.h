#pragma once
#include <string>

#include "Renderer.h"
#include "Texture.h"


class Shader
{
	private:
		unsigned int m_renderer_id;
        const char* m_vertex_path;
        const char* m_fragment_path;

	public:
		Shader(const char* vertex_path, const char* fragment_path);
		~Shader();
	
		void Bind() const;
		void Unbind() const;

		// utility uniform functions
		bool GetUniformLocation(const std::string& name, int& location) const;
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVec3(const std::string& name, const float x, const float y, const float z);
		void SetVec4(const std::string& name, const float x, const float y, const float z, const float w);
		void SetMat4(const std::string& name, glm::mat4 value);

        void PrintShaderFiles() const;
};
