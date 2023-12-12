#version 330 core

// Task 5: declare "in" variables for the world-space position and normal,
//         received post-interpolation from the vertex shader
in vec3 m_world_pos;
in vec3 m_world_norm;
in vec2 uv_out;
in float obj_index;

// Task 10: declare an out vec4 for your output color
out vec4 fragColor;

// Task 12: declare relevant uniform(s) here, for ambient lighting
uniform float m_ka;

// Task 13: declare relevant uniform(s) here, for diffuse lighting
uniform float m_kd;
uniform vec4 light_pos;

// Task 14: declare relevant uniform(s) here, for specular lighting
uniform float m_ks;
uniform float shininess;
uniform vec4 cam_pos;

void main() {
    // Remember that you need to renormalize vectors here if you want them to be normalized

    // Task 10: set your output color to white (i.e. vec4(1.0)). Make sure you get a white circle!
//    fragColor = vec4(1.0);

//    // Task 11: set your output color to the absolute value of your world-space normals,
//    //          to make sure your normals are correct.
//    fragColor = vec4(abs(m_world_norm), 1.0);
    int int_obj = int(obj_index);
    if (int_obj != 2) {
        // Task 12: add ambient component to output color
        fragColor = vec4(m_ka) * vec4(0.0,0.5, 1.0, 1.0);

        // Task 13: add diffuse component to output color
        vec3 normN = normalize(m_world_norm);

        vec3 posToLight = normalize(vec3(light_pos) - m_world_pos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0));

        // Task 14: add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - m_world_pos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);

        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
    else {
        discard;
    }

}
