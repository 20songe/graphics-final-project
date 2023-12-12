#version 330 core

// Task 15: add a second layout variable representing a UV coordinate
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_in;
layout (location = 2) in vec3 normal;
layout (location = 3) in float obj;

// Task 16: create an "out" variable representing a UV coordinate
//out vec2 uv_out;
out vec3 vertPos;
out vec3 out_normal;
out float obj_index;

uniform mat4 proj;
uniform mat4 view;
uniform float time;
uniform vec4 center;

void main() {

//    vertPos = position;

//    out_normal = normal;
//    obj_index = obj;

//    vec4 world_pos4 = vec4(position, 1.0);
//    gl_Position = proj * view * world_pos4;
    if (int(obj) == 1 || time < 0) {
        vertPos = position;

        out_normal = normal;
        obj_index = obj;

        vec4 world_pos4 = vec4(position, 1.0);
        gl_Position = proj * view * world_pos4;

    }
    else if (int(obj) == 2 && time >= 0) {

        // source: https://medium.com/@joshmarinacci/water-ripples-with-vertex-shaders-6a9ecbdf091f

        // dx and dz are distances from a certain point
//        vec4 center = vec4(-3.0, 0.0, 5.0, 1.0);

        float dx = length(position.x - center);
        float dz = length(position.z - center);

//        float freq = 2.0 * sqrt(dx * dx + dz * dz);
//        float amp = 1.0 - 1.0 / (0.65 * freq);
        float freq = 0.2 * sqrt(dx * dx + dz * dz);
        float amp = max(0, 1.0 - 1.0 / (0.65 * freq) - time * 1 / 5);
        float angle = -time * 10.0 + freq * 6.0;
        vertPos = position;
        vertPos.y += sin(angle) * amp / freq;

        out_normal = normalize(vec3(0.0, 1.0, -amp * freq * cos(angle)));

        gl_Position = proj * view * vec4(position, 1.0);
    }


}
