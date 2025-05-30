#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;

// Output to fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(position, 1.0);
    FragPos = vec3(worldPos); // World space position
    Normal = mat3(transpose(inverse(model))) * normal; // Normal matrix
    Color = color;

    gl_Position = projection * view * worldPos;
}