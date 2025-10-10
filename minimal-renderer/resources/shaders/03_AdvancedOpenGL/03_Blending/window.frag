#version 330 core
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

struct Material{
	vec3 base_color;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform vec3 u_viewPos;
uniform Material material;

void main()
{
	// FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	FragColor = texture(material.diffuse, TexCoords);
}
