#version 330 core

// Task 15: add a second layout variable representing a UV coordinate
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_in;
layout (location = 2) in vec3 normal;
layout (location = 3) in float obj;

// Task 16: create an "out" variable representing a UV coordinate
out vec2 uv_out;
out vec3 vertPos;
out vec3 out_normal;
out float obj_index;

uniform mat4 proj;
uniform mat4 view;

void main() {
    // Task 16: assign the UV layout variable to the UV "out" variable
    uv_out = uv_in;
    vertPos = position;

    out_normal = normal;
    obj_index = obj;

    vec4 world_pos4 = vec4(position, 1.0);
    gl_Position = proj * view * world_pos4;
}
