#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec4 scene_out = texture(screenTexture, TexCoords);

    // grayscale
    vec3 luma709 = vec3(0.2126, 0.7152, 0.0722);
    float avg = dot(luma709, scene_out.rgb);
    
    FragColor = vec4(avg, avg, avg, 1.0);
}