#version 330 core
layout (location = 0) in vec3 position; // <vec3 position
//layout (location = 1) in vec2 uv;

//out vec2 TexCoords;
//out vec4 ParticleColor;

//uniform mat4 projection;
// Task 6: declare a uniform mat4 to store model matrix
uniform mat4 model;

// Task 7: declare uniform mat4's for the view and projection matrix
uniform mat4 view;
uniform mat4 proj;
uniform vec3 offset;
uniform vec4 color;

void main()
{
    float scale = 10.0f;
    //TexCoords = uv;
    //ParticleColor = color;
    gl_Position = proj*view*model*vec4((position) + offset, 1.0);
}
