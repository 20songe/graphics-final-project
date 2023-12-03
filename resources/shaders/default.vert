#version 330 core

// declare a vec3 object-space position variable, using
//         the `layout` and `in` keywords.
layout(location = 0) in vec3 m_object_pos;
layout(location = 1) in vec3 m_object_norm;

// declare `out` variables for the world-space position and normal,
//         to be passed to the fragment shader
out vec3 m_world_pos;
out vec3 m_world_norm;

// declare a uniform mat4 to store model matrix
uniform mat4 model;
uniform mat3 m3;

// declare uniform mat4's for the view and projection matrix
uniform mat4 view;
uniform mat4 proj;

void main() {
    // compute the world-space position and normal, then pass them to
    //         the fragment shader using the variables created in task 5

    vec4 world_pos4 = model * vec4(m_object_pos, 1.0);
    m_world_pos = vec3(world_pos4);

    m_world_norm = m3 * normalize(m_object_norm);

    // set gl_Position to the object space position transformed to clip space
    gl_Position = proj * view * world_pos4;
}
