#version 330 core

in vec3 m_world_pos;
in vec3 m_world_norm;

// Output color
out vec4 fragColor;

// Number of lights
uniform int lightCount;

// Uniforms for ambient lighting
uniform float m_ka;
uniform vec4 m_ambient;

// Uniforms for diffuse lighting
uniform float m_kd;
uniform vec4 m_diffuse;

// Uniforms here, for specular lighting
uniform float m_ks;
uniform float shininess;
uniform vec4 m_specular;
uniform vec4 cam_pos;

// Uniforms for lights
uniform vec4 light_illumination[8];
uniform vec4 light_dir[8];
uniform vec4 light_pos[8];
uniform vec3 function[8];
uniform int light_type[8];
uniform float light_angle[8];
uniform float light_penumbra[8];

float falloff(float angle, float inner, float outer) {
    return -2 * pow((angle - inner) / (outer - inner), 3) + 3 * pow((angle - inner) / (outer - inner), 2);
}

void main() {

        fragColor = m_ka * m_ambient;

        for (int i = 0; i < lightCount; i++) {

            vec4 illumination = light_illumination[i];

            vec4 normN = vec4(normalize(m_world_norm), 0);

            float attenuation = 1.0;
            float dist = 1.0;
            vec4 li = vec4(0.0);

            float c1 = function[i][0];
            float c2 = function[i][1];
            float c3 = function[i][2];

            int type = light_type[i];

            if (type == 0) {
                li = normalize(-light_dir[i]);

            }
            else if (type == 1) {
                li = normalize(light_pos[i] - vec4(m_world_pos, 1.0));

                dist = length(vec3(light_pos[i]) - m_world_pos);
                attenuation = min(1.0, 1.0 / (c1 + dist * c2 + c3 * dist * dist));
            }
            else {
                li = normalize(light_pos[i] - vec4(m_world_pos, 1.0));

                dist = length(vec3(light_pos[i]) - m_world_pos);
                attenuation = min(1.0, 1.0 / (c1 + dist * c2 + c3 * dist * dist));

                float angle = acos(dot(-li, light_dir[i]) / (length(-li) * length(light_dir[i])));
                float outer = light_angle[i];
                float inner = light_angle[i] - light_penumbra[i];
                if (angle > inner && angle <= outer) {
                    illumination = (1.0 - falloff(angle, inner, outer)) * illumination;
                }
                else if (angle > outer) {
                    illumination = vec4(0.0);
                }
            }

            fragColor += attenuation * illumination * m_kd * m_diffuse * max(0, dot(normN, li));

            vec4 ri = reflect(-li, normN);
            vec4 v = normalize(cam_pos - vec4(m_world_pos, 1.0));

            float x = max(dot(ri, v), 0);
            if (x == 0 && shininess <= 0) {
                continue;
            }
            else {
                fragColor += attenuation * illumination * m_ks * m_specular * pow(x, shininess);
            }

        }

        fragColor = clamp(fragColor, 0.0, 1.0);
}
