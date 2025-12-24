#version 330 core
layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 mvp;

out VS_OUT
{
    vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    vs_out.TexCoords = aTexCoords;
}

