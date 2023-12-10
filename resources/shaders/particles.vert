#version 330 core
layout (location = 0) in vec3 position; // <vec4 position>
//TODO add texture in variable

out vec4 ParticleColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    ParticleColor = color;
    gl_Position =  proj * view * model * vec4(position, 1.0);
}
