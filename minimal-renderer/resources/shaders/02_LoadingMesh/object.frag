#version 330 core
#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

struct Material {
	vec3 base_color;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cut_off;
	float outer_cut_off;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 u_viewPos;
uniform DirLight dir_light;
uniform PointLight pt_lights[NR_POINT_LIGHTS];
uniform SpotLight spot_lights[NR_SPOT_LIGHTS];
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 frag_pos);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 frag_pos);

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 view_dir = normalize(u_viewPos - FragPos);

	// directional light
	vec3 result = CalcDirLight(dir_light, norm, view_dir);
	// // point lights
	// for (int i = 0; i < NR_POINT_LIGHTS; i++)
	// { 
	// 	result += CalcPointLight(pt_lights[i], norm, view_dir, FragPos); 
	// }
	// // spot lights
	// for (int i = 0; i < NR_SPOT_LIGHTS; i++)
	// {
	// 	result += CalcSpotLight(spot_lights[i], norm, view_dir, FragPos);
	// }

	// final color
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 view_dir)
{
	// flipped to get fragment -> light vector
	vec3 light_dir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, light_dir), 0.0);

	// specular shading
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// combine result
	vec3 ambient  = light.ambient  * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 view_dir, vec3 frag_pos)
{
	vec3 light_dir = normalize(light.position-frag_pos);

	// diffuse shading
	float diff = max(dot(normal, light_dir), 0.0);

	// specular shading
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// attenuation
	float dist		  = length(light.position - frag_pos);
	float attenuation = 1.0 / ( light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// combine result
	vec3 ambient  = light.ambient  * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 view_dir, vec3 frag_pos)
{

	// diffuse shading
	vec3 light_dir = normalize(light.position-frag_pos);
	float diff = max(dot(normal, light_dir), 0.0);

	// specular shading
	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// spotlight
	float theta		= dot(light_dir, normalize(-light.direction));				// angle between LightDir vector & SpotDir vector
	float epsilon	= light.cut_off - light.outer_cut_off;						// cosine difference between the inner & outer cone
	float intensity	= clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	// attenuation
	float dist = length(light.position-FragPos);
	float attenuation = 1.0/ (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	// combine result
	vec3 ambient  = light.ambient  * texture(material.diffuse,TexCoords).rgb;
	vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse,TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

	diffuse *= intensity;
	specular *= intensity;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}
