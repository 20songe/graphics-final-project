#version 330 core

in vec3 m_world_pos;
in vec3 m_world_norm;
in vec2 uv_out;

out vec4 fragColor;

//Lighting Related Uniforms
uniform float m_ka;
uniform float m_kd;
uniform vec4 light_pos;
uniform float m_ks;
uniform float shininess;
uniform vec4 cam_pos;

//Water related uniforms
uniform int isWater;

void main() {
//     Remember that you need to renormalize vectors here if you want them to be normalized
//    fragColor = vec4(1.0);

    if (isWater == 1) {
           // Logic specific to water rendering
           // You can sample from a texture if you're using one
           // Implement lighting effects based on normals if needed
           fragColor = vec4(0.0, 0.0, 1.0, 1.0); // Placeholder color
       } else {
        fragColor = vec4(abs(m_world_norm), 1.0);

        // Task 12: add ambient component to output color
        fragColor = vec4(m_ka);

        // Task 13: add diffuse component to output color
        vec3 normN = normalize(m_world_norm);

        vec3 posToLight = normalize(vec3(light_pos) - m_world_pos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0));

        // Task 14: add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - m_world_pos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);

        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
}
