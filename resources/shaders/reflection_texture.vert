#version 330 core

//add a second layout variable representing a UV coordinate
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_in;
layout (location = 2) in vec3 normal;
layout (location = 3) in float obj;

//create an "out" variable representing a UV coordinate
out vec3 vertPos;
out vec3 out_normal;
out float obj_index;
out vec2 textureCoord;
out vec4 world_pos4;

uniform mat4 proj;
uniform mat4 view;
uniform float time;
uniform vec4 center;

const float tiling = 6.0;

void main() {
    if (int(obj) == 1 || time < 0) {
        vertPos = position;
        out_normal = normal;
        obj_index = obj;
        world_pos4 = vec4(position, 1.0);
        gl_Position = proj * view * world_pos4;

        textureCoord = vec2(uv_in.x,uv_in.y) * tiling;

    }
    else if (int(obj) == 2 && time >= 0) {

        float dx = length(position.x - center.x);
             float dz = length(position.z - center.z);

             float dist = sqrt(dx * dx + dz * dz);
             float radius = 10.0;
             if (dist <= radius) {
                 float freq = 0.5 * dist;
                 float amp = 0.1;
                 float angle = -time * 10.0 + freq * 6.0;
                 vertPos = position;
                 vertPos.y += sin(angle) * amp;

                 out_normal = normalize(vec3(0.0, 1.0, -amp * freq * cos(angle)));
             }
             else {
                 out_normal = normalize(normal);
                 vertPos = position;
             }


        gl_Position = proj * view * vec4(vertPos, 1.0);
    }


}
