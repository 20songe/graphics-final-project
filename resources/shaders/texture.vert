#version 330 core

layout (location = 0) in vec3 position;

layout (location = 1) in vec2 uvcoord;

out vec2 coordinate;

void main() {
    //assign the UV layout variable to the UV "out" variable
    gl_Position = vec4(position, 1.0);
    coordinate = uvcoord;
}
