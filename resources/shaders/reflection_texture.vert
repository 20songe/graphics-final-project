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

uniform mat4 proj;
uniform mat4 view;
uniform float time;
uniform vec4 center;

const float tiling = 6.0;

const float PI = 3.14159;

void main() {
    if (int(obj) == 1 || time < 0) {
        vertPos = position;
        out_normal = normal;
        obj_index = obj;
        vec4 world_pos4 = vec4(position, 1.0);
        gl_Position = proj * view * world_pos4;
        //but we want object space here

        vec3 e1 = normalize(cross(normal, vec3(1, 0, 0)));

        //If normal and (1,0,0) are parallel, change e1
        if (e1 == vec3(0, 0, 0)) {
           vec3 e1 = normalize(cross(normal, vec3(0, 0, 1)));
        }
        vec3 e2 = normalize(cross(normal, e1));

        float u = dot(e1, position);
        float v = dot(e2, position);

//        textureCoord = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5);
        textureCoord = vec2(u,v);

    }
    else if (int(obj) == 2 && time >= 0) {

        float dx = length(position.x - center.x); //probably here is the problem
             float dz = length(position.z - center.z);

             float dist = sqrt(dx * dx + dz * dz);
             float radius = 10.0; //need to decrease this
             if (dist <= radius) {
                 float freq = 0.5 * dist;
     //            float amp = min(1, max(0, 1.0 - 1.0 / (0.65 * freq) - time * 1 / 5));
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
