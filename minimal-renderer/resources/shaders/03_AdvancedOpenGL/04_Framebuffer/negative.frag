#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 scene_out = texture(screenTexture, TexCoords);
    
    FragColor = vec4(1 - scene_out.rgb, 1.0);
}