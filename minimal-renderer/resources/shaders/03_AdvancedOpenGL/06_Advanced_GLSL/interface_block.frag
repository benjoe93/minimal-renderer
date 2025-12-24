#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec2 TexCoords;
} fs_in;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, fs_in.TexCoords);
}