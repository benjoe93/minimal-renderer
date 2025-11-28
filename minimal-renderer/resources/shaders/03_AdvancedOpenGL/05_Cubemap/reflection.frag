#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 camera_position;
uniform samplerCube cubemap;

void main() {
    // calculate the view direction vector
    vec3 I = normalize(Position - camera_position);
    // calculate the reflection vector
    vec3 R = reflect(I, normalize(Normal));
    // out color
    FragColor = vec4(texture(cubemap, R).rgb, 1.0);
}