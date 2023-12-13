#version 330 core

// UV coordinate variable
//in vec2 uv_out;
in vec3 vertPos;
in vec3 out_normal;
in float obj_index;
in vec2 textureCoord;
in vec4 world_pos4;

// Sampler2D variable
uniform sampler2D texSampler;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;

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

const float waveStrength = 0.01;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

vec4 blend(vec4 textureColor, vec4 diffuseColor, float blend) {
    return blend * textureColor + (1.f - blend) * diffuseColor;
}

void main(){

    int int_obj = int(obj_index);
    float width_step = 1.f / float(width);
    float height_step = 1.f / float(height);

    vec3 toCameraVector = vec3(cam_pos) - world_pos4.xyz;
    vec3 fromLightVector = world_pos4.xyz - vec3(light_pos);


    if (int_obj == 2) { //do water reflection
        fragColor = vec4(0);
        vec2 uv = vec2 (gl_FragCoord.x / width, gl_FragCoord.y / height);
        for (int i = -4; i <= 4; i++) {
            for (int j = -4; j <= 4; j++) { //I guess this is blurring?

                float u = float(i) * float(width_step) + uv.x;
                float v = float(j) * float(height_step) + uv.y;

                //add DUDV map:
                vec2 distortion1 = (texture(dudvMap,vec2(textureCoord.x, textureCoord.y)).rg * 2.0 - 1.0)*waveStrength;
                vec2 distorted_uv = vec2(uv) + distortion1;

                //add normal map:
                vec4 normal_color = texture(normalMap,distorted_uv);
                vec3 normal = vec3(normal_color.r*2.0-1.0,normal_color.b, normal_color.g*2.0 - 1.0);

                vec3 reflectedLight = reflect(normalize(fromLightVector),normal);
                float specular = max(dot(reflectedLight,normalize(toCameraVector)),0.0);
                specular = pow(specular, shineDamper);
                vec3 specularHighlights = vec3(1.0,1.0,1.0) * specular * reflectivity;


                vec4 diffuse = texture(texSampler, distorted_uv);
//                vec4 diffuse = texture(normalMap, distorted_uv);
//                vec4 diffuse = texture(dudvMap, vec2(u, v));
//                  vec4 diffuse = texture(texSampler, vec2(u, v));
                vec4 blended = blend(diffuse, vec4(0, 0, 1, 1), 0.7) + vec4(specularHighlights,0.0);



                fragColor += blended;
            }
        }

        //add ambient component to output color
        fragColor = fragColor / 81;

        //add diffuse component to output color
        vec3 normN = normalize(out_normal);

        vec3 posToLight = normalize(vec3(light_pos) - vertPos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0) * vec4(0,0,1,0.5));

        //add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - vertPos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);

        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
    else if (int_obj == 1) {
        //add ambient component to output color
        fragColor = vec4(m_ka) * vec4(0.0,0.5, 1.0, 1.0);

        //add diffuse component to output color
        vec3 normN = normalize(out_normal);

        vec3 posToLight = normalize(vec3(light_pos) - vertPos);
        fragColor += vec4(m_kd * clamp(dot(normN, posToLight), 0.0, 1.0));

        //add specular component to output color
        vec3 posToCam = normalize(vec3(cam_pos) - vertPos);
        vec3 reflection = normalize(-posToLight - 2.0 * dot(normN, -posToLight) * normN);
        fragColor += vec4(m_ks * pow(clamp(dot(reflection, posToCam), 0.0, 1.0), shininess));
    }
    else {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }

}
