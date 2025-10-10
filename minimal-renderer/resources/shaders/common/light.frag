#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_texture;
uniform vec3 u_lightColor;

void main()
{
    vec4 texColor = texture(u_texture, TexCoords);
    if (texColor.a < 0.1)
        discard;

	FragColor = texColor * vec4(u_lightColor, 1.0);
}