#version 330 core

// UV coordinate variable
//in vec2 uv_out;
in vec3 vertPos;
in vec3 out_normal;
in float obj_index;

// Sampler2D variable
uniform sampler2D texSampler;

uniform float width;
uniform float height;
//uniform mat4 reflection_projection;

out vec4 fragColor;

//declare relevant uniform(s) here, for ambient lighting
uniform float m_ka;

//declare relevant uniform(s) here, for diffuse lighting
uniform float m_kd;
uniform vec4 light_pos;

//declare relevant uniform(s) here, for specular lighting
uniform float m_ks;
uniform float shininess;
uniform vec4 cam_pos;

vec4 blend(vec4 textureColor, vec4 diffuseColor, float blend) {
    return blend * textureColor + (1.f - blend) * diffuseColor;
}

void main()
{

    int int_obj = int(obj_index);
    float width_step = 1.f / float(width);
    float height_step = 1.f / float(height);

    if (int_obj == 2) {
        fragColor = vec4(0);
        vec2 uv = vec2 (gl_FragCoord.x / width, gl_FragCoord.y / height);
        for (int i = -4; i <= 4; i++) {
            for (int j = -4; j <= 4; j++) {

                float u = float(i) * float(width_step) + uv.x;
                float v = float(j) * float(height_step) + uv.y;

                vec4 diffuse = texture(texSampler, vec2(u, v));
                vec4 blended = blend(diffuse, vec4(0, 0, 1, 1), 0.7);

                fragColor += blended;
            }
        }


//        // Task 12: add ambient component to output color
//        fragColor = vec4(m_ka);
        fragColor = fragColor / 81;

//        // Task 13: add diffuse component to output color
        vec3 normN = normalize(out_normal);

        vec3 posToLight = normalize(vec3(light_pos) - vertPos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0) * vec4(0,0,1,0.5));

//        // Task 14: add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - vertPos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);

        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
    else if (int_obj == 1) {
        // Task 12: add ambient component to output color
        fragColor = vec4(m_ka) * vec4(0.0,0.5, 1.0, 1.0);

        // Task 13: add diffuse component to output color
        vec3 normN = normalize(out_normal);

        vec3 posToLight = normalize(vec3(light_pos) - vertPos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0));

        // Task 14: add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - vertPos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);

        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
    else {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }

}
