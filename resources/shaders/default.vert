#version 330 core

// Task 4: declare a vec3 object-space position variable, using
//         the `layout` and `in` keywords.
layout(location = 0) in vec3 m_object_pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Task 5: declare `out` variables for the world-space position and normal,
//         to be passed to the fragment shader
out vec3 m_world_pos;
out vec2 uv_out;
out vec3 m_world_norm;

// Task 6: declare a uniform mat4 to store model matrix
uniform mat4 model;

// Task 7: declare uniform mat4's for the view and projection matrix
uniform mat4 view;
uniform mat4 proj;

uniform float time;

void main() {
    // Task 8: compute the world-space position and normal, then pass them to
    //         the fragment shader using the variables created in task 5

    // source: https://medium.com/@joshmarinacci/water-ripples-with-vertex-shaders-6a9ecbdf091f
    vec4 world_pos4 = model * vec4(m_object_pos, 1.0);

    float dx = m_object_pos.x;
    float dz = m_object_pos.z;
    float freq = sqrt(dx * dx + dz * dz);
    float amp = 1.0;
    float angle = -time * 10.0 + freq * 6.0;
    m_world_pos = m_object_pos;
    m_world_pos[1] += sin(angle) * amp;

    m_world_norm = normalize(vec3(0.0,-amp * freq * cos(angle),1.0));

    uv_out = uv;

    // Recall that transforming normals requires obtaining the inverse-transpose of the model matrix!
    // In projects 5 and 6, consider the performance implications of performing this here.

    // Task 9: set gl_Position to the object space position transformed to clip space
    gl_Position = proj * view * world_pos4;
}
