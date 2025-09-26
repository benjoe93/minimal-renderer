#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 u_viewPos;

uniform Material material;
uniform Light light;

void main()
{
	/* Ambient light */
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	/* Diffuse light */
	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(light.position - FragPos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	/* Specular light */
	vec3 view_dir = normalize(u_viewPos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	/* Final output */
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}