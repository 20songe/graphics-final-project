#version 330 core

//object space variables
layout(location = 0) in vec3 m_object_pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

//out to be passed in the fragment shader
out vec3 m_world_pos;
out vec2 uv_out;
out vec3 m_world_norm;

// Uniforms:
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//uniform int isWater; // Uniform to switch between water and general object rendering

void main() {
//    if (isWater == 1) {
//          // Logic for water vertices
//          vec4 world_pos4 = model * vec4(m_object_pos.x, 0.0, m_object_pos.y, 1.0);
//          m_world_pos = vec3(world_pos4);
//          uv_out = uv; // Use texture coordinates directly
//          m_world_norm = normal; // Use normals directly
//          gl_Position = proj * view * world_pos4;
//      } else {
        vec4 world_pos4 = model * vec4(m_object_pos, 1.0);
        m_world_pos = vec3(world_pos4);
        m_world_norm = normal;
        uv_out = uv;
    // Task 9: set gl_Position to the object space position transformed to clip space
        gl_Position = proj * view * world_pos4;
//    }
}
