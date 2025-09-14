#version 330 core

out vec4 FragColor;

in vec3 lightColor;
in vec3 objectColor;

void main()
{
	vec3 result = lightColor * objectColor;
	FragColor = vec4(result, 1.0);
}