
// --- student code ---

#version 330 core

in vec3 worldSpacePos;
in vec3 worldSpaceNormal;

out vec4 fragColor;

uniform vec4 worldSpaceCameraPos;

uniform float m_ka;
uniform float m_kd;
uniform float m_ks;

uniform float m_shininess;
uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;

// lights
uniform int m_numDirLights;
uniform int m_numPointLights;
uniform int m_numSpotLights;

uniform vec4 m_dirLightDirs[8];
uniform vec4 m_dirLightColors[8];

uniform vec4 m_pointLightPos[8];
uniform vec4 m_pointLightColors[8];
uniform vec3 m_pointLightAttenuation[8];

uniform vec4 m_spotLightPos[8];
uniform vec4 m_spotLightDirs[8];
uniform vec4 m_spotLightColors[8];
uniform vec3 m_spotLightAttenuation[8];
uniform float m_spotLightAngle[8];
uniform float m_spotLightP[8];

// texture mapping
in vec2 obj_uv;
in float materialIndex;

uniform sampler2D m_texture0;
uniform sampler2D m_opacity_texture0;
uniform sampler2D m_normal_texture0;

uniform sampler2D m_texture1;
uniform sampler2D m_opacity_texture1;
uniform sampler2D m_normal_texture1;

// falloff function
float falloff(float x, float angle, float penumbra) {
    float thetaInner = angle - penumbra;
    if (thetaInner > angle) return 1.f;
    if (x < thetaInner) return 0.f;
    return -2.f * pow((x - thetaInner) / penumbra, 3.f) + 3.f * pow((x - thetaInner) / penumbra, 2.f);
}

// rotation about axis
mat3 rotationMatrix(vec3 axis, float angle) {

    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(
        oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c
    );

}

// angle between vectors
float angle(vec3 x, vec3 y) {
    float num = dot(x, y);
    float dem = length(x) * length(y);
    return acos(num / dem);
}

void main() {

//    // settings textures
//    vec4 m_texture;
//    vec4 m_opacity_texture;
//    vec4 m_normal_texture;
//    switch (int(materialIndex)) {
//        case 0: { // bark
//            m_texture = texture(m_texture1, obj_uv);
//            m_opacity_texture = texture(m_opacity_texture1, obj_uv);
//            m_normal_texture = texture(m_normal_texture1, obj_uv);
//            break;
//        }
//        case 1: { // leaf
//            m_texture = texture(m_texture0, obj_uv);
//            m_opacity_texture = texture(m_opacity_texture0, obj_uv);
//            m_normal_texture = texture(m_normal_texture0, obj_uv);
//            break;
//        }
//    }

    vec4 m_texture = texture(m_opacity_texture1, obj_uv);
    vec4 m_opacity_texture = texture(m_opacity_texture1, obj_uv);
    vec4 m_normal_texture = texture(m_opacity_texture1, obj_uv);

    // texture coloring
    vec4 textureColor = m_texture;

    // normal mapping
    float strength = 5.f;

    vec3 up = vec3(0.f, 0.f, 1.f); // -> worldSpaceNormal
    vec3 axis = cross(up, worldSpaceNormal);
    mat3 transform = rotationMatrix(axis, -angle(up, worldSpaceNormal));

    vec3 mapNormal = normalize(2 * vec3(m_normal_texture) - vec3(1.f));
    vec3 texturedNormal = normalize(worldSpaceNormal + strength * transform * mapNormal);

    // phong lighting

    // ambient
    fragColor = m_ka * cAmbient;

    for (int i = 0; i < m_numDirLights; i++) { // directional lighing

        // difffuse component
        vec3 dir = -normalize(vec3(m_dirLightDirs[i]));

        // diffuse texture color
        fragColor = fragColor + clamp(dot(dir, texturedNormal), 0.0, 1.0) * m_dirLightColors[i] * textureColor;

        // no texture
        // fragColor = fragColor + m_kd * clamp(dot(dir, texturedNormal), 0.0, 1.0) * m_dirLightColors[i] * cDiffuse;

        // specular component
        vec3 surfToLight = dir; // this is right!!
        vec3 reflected = surfToLight - 2.0 * dot(texturedNormal, surfToLight) * texturedNormal;
        vec3 surfToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos); // this is right...
        float value = clamp(dot(-reflected, surfToCamera), 0.0, 1.0);

        // check bounds
        if (value <= 0.0) continue;
        if (m_shininess <= 0.0) continue;

        // specular
        fragColor = fragColor + m_ks * pow(value, m_shininess) * m_dirLightColors[i] * cSpecular;

    }

    for (int i = 0; i < m_numPointLights; i++) { // point lighting

        // attenuation
        float dist = distance(worldSpacePos, vec3(m_pointLightPos[i]));
        float f = min(1.f, 1.f / (m_pointLightAttenuation[i][0] + dist * m_pointLightAttenuation[i][1] + dist * dist * m_pointLightAttenuation[i][2]));

        // diffuse
        vec3 posToLightDir = normalize(vec3(m_pointLightPos[i]) - worldSpacePos);
        float dotDiffuse = clamp(dot(texturedNormal, posToLightDir), 0.f, 1.f);
        vec4 diffuse = m_kd * dotDiffuse * cDiffuse;
        fragColor = fragColor + m_pointLightColors[i] * diffuse * f;

        // specular
        vec3 directionToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
        vec3 reflect = normalize(reflect(-posToLightDir, texturedNormal));
        float dotSpec = clamp(dot(reflect, directionToCamera), 0.f, 1.f);

        if (dotSpec <= 0.0) continue;
        if (m_shininess <= 0.0) continue;

        vec4 specular = m_ks * cSpecular * pow(dotSpec, m_shininess);
        fragColor = fragColor + m_pointLightColors[i] * specular * f;

    }

    for (int i = 0; i < m_numSpotLights; i++) {

        // attenuation
        float dist = distance(worldSpacePos, vec3(m_pointLightPos[i]));
        float f = min(1.f, 1.f / (m_spotLightAttenuation[i][0] + dist * m_spotLightAttenuation[i][1] + dist * dist * m_spotLightAttenuation[i][2]));

        // falloff
        vec3 lightToPosDir = normalize(worldSpacePos - vec3(m_spotLightPos[i]));
        float angleDif = acos(dot(lightToPosDir, vec3(normalize(m_spotLightDirs[i]))));
        float falloffValue = 1 - falloff(angleDif, m_spotLightAngle[i], m_spotLightP[i]);
        if (angleDif >= m_spotLightAngle[i]) continue; // stop if outside the angle

        // diffuse
        vec3 posToLightDir = normalize(vec3(m_spotLightPos[i]) - worldSpacePos);
        float dotDiffuse = clamp(dot(texturedNormal, posToLightDir), 0.f, 1.f);
        vec4 diffuse = m_kd * dotDiffuse * cDiffuse;
        fragColor = fragColor + m_spotLightColors[i] * diffuse * f * falloffValue;

        // specular
        vec3 directionToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos);
        vec3 reflect = normalize(reflect(-posToLightDir, texturedNormal));
        float dotSpec = clamp(dot(reflect, directionToCamera), 0.f, 1.f);

        if (dotSpec <= 0.0) continue;
        if (m_shininess <= 0.0) continue;

        vec4 specular = m_ks * cSpecular * pow(dotSpec, m_shininess);
        fragColor = fragColor + m_spotLightColors[i] * specular * f * falloffValue;

    }

    // opacity mapping
    vec4 opacity = m_opacity_texture;
    fragColor[3] = opacity[0];

}

// --- student code end ---
