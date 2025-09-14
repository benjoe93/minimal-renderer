#version 330 core

layout (location=0) in vec3 aPos;

out vec3 lightColor;
out vec3 objectColor;

uniform vec3 inLightColor;
uniform vec3 inObjectColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	lightColor = inLightColor;
	objectColor = inObjectColor;
}