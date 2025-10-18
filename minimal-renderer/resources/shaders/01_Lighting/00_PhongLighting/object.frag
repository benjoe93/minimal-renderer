#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 u_objectColor;

uniform vec3 u_viewPos;

uniform vec3 u_lightColor;
uniform vec3 u_lightPos;

float SPECULAR_STREGTH = 0.5;
float SHINE_FACTOR = 32.0;

void main()
{
	/* Ambient light */
	float ambientStrenght = 0.2;
	vec3 ambient = ambientStrenght * u_objectColor;

	/* Diffuse light */
	vec3 norm = normalize(Normal);
	vec3 light_dir = normalize(u_lightPos - FragPos);

	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * u_lightColor;

	/* Specular light */
	vec3 view_dir = normalize(u_viewPos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), SHINE_FACTOR);
	vec3 specular = SPECULAR_STREGTH * spec * u_lightColor;

	/* Final output */
	vec3 result = (ambient + diffuse + specular) * u_objectColor;
	FragColor = vec4(result, 1.0);

}