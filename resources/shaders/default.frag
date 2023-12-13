
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

uniform sampler2D m_texture;
uniform sampler2D m_opacity_texture;
uniform sampler2D m_normal_texture;

// water ripples
uniform bool m_water;
uniform int m_numWaterPoints;
uniform vec4[8] m_waterPointCenters;
uniform float[8] m_waterPointElapsedTimes;

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

// calculate ripple normals for water
vec3 rippleNormal() {

    // return value
    vec3 outNormal = vec3(0.f);

    // changeable values
    float movementSpeed = 0.5f;
    float frequency = 5.f;

    float distFalloffStrength = 1.f;
    float timeFalloffStrength = 1e-1f;

    for (int i = 0; i < m_numWaterPoints; i++) {

        // distance and direction from water point center...
        float dist = distance(vec3(m_waterPointCenters[i]), worldSpacePos);
        vec2 direction = normalize(vec2(worldSpacePos[0], worldSpacePos[2]) - vec2(m_waterPointCenters[i][0], m_waterPointCenters[i][2]));

        // ripple value and derivative
        float amplitude = sin((dist - m_waterPointElapsedTimes[i] * movementSpeed) * frequency * 2.f * 3.14f);
        float slope = cos((dist - m_waterPointElapsedTimes[i] * movementSpeed) * frequency * 2.f * 3.14f);

        // pseudo normals calculation
        vec3 sideNormal = slope * vec3(direction[0], 0.f, direction[1]);
        float sideNormalMag = length(sideNormal);
        vec3 undampedNormal = normalize(sideNormal + (1.f - sideNormalMag) * vec3(0.f, 1.f, 0.f));

        // damping
        float damping = 0.f;
        if (dist < m_waterPointElapsedTimes[i] * movementSpeed - 0.25f) damping = 1.f;
        if (dist > m_waterPointElapsedTimes[i] * movementSpeed + 0.125f) damping = 1.f;

        vec3 dampedNormal = (1.f - damping) * undampedNormal + damping * vec3(0.f, 1.f, 0.f);

        damping = clamp(dist * dist, 0.f, 1.f); // distance falloff
        dampedNormal = (1.f - damping) * dampedNormal + damping * vec3(0.f, 1.f, 0.f);

        // bounds checking
        if (dist < 1e-4f) dampedNormal = vec3(0.f , 1.f, 0.f);

        // superimpose normals
        outNormal += normalize(dampedNormal);

    }

    // faking wave strength...
    return 1.25f * normalize(outNormal);

}

void main() {

    // turns on and off texture mapping
    bool textureMapping = true;
    bool normalMapping = true;
    bool opacityMapping = true;

    // turn off textures for water...
    if (m_water) {
        textureMapping = false;
        normalMapping = false;
        opacityMapping = false;
    }

    // texture coloring
    vec4 textureColor = texture(m_texture, obj_uv);

    // normal mapping
    vec3 texturedNormal = normalize(worldSpaceNormal);
    if (normalMapping) { // check for normal mapping
        float strength = 5.f;

        vec3 up = vec3(0.f, 0.f, 1.f); // -> worldSpaceNormal
        vec3 axis = cross(up, worldSpaceNormal);
        mat3 transform = rotationMatrix(axis, -angle(up, worldSpaceNormal));

        vec3 mapNormal = normalize(2 * vec3(texture(m_normal_texture, obj_uv)) - vec3(1.f));
        texturedNormal = normalize(worldSpaceNormal + strength * transform * mapNormal);
    }

    // edit normals for water
    if (m_water) {
        texturedNormal = rippleNormal();
    }

    // phong lighting

    float blend = 0.5f;

    // ambient
    fragColor = m_ka * cAmbient;

    for (int i = 0; i < m_numDirLights; i++) { // directional lighing

        // difffuse component
        vec3 dir = -normalize(vec3(m_dirLightDirs[i]));

        // diffuse color
        if (textureMapping) { // check for texture mapping
            float dotDiffuse = clamp(dot(dir, texturedNormal), 0.f, 1.f);
            vec4 diffuse = (m_kd * cDiffuse * (1.f - blend) + textureColor * blend) * dotDiffuse;
            fragColor = fragColor + m_dirLightColors[i] * diffuse;
        } else {
            fragColor = fragColor + m_kd * clamp(dot(dir, texturedNormal), 0.f, 1.f) * m_dirLightColors[i] * cDiffuse;
        }

        // specular component
        vec3 surfToLight = dir; // this is right!!
        vec3 reflected = surfToLight - 2.f * dot(texturedNormal, surfToLight) * texturedNormal;
        vec3 surfToCamera = normalize(vec3(worldSpaceCameraPos) - worldSpacePos); // this is right...
        float value = clamp(dot(-reflected, surfToCamera), 0.f, 1.f);

        if (value <= 0.f) continue;
        if (m_shininess <= 0.f) continue;

        fragColor = fragColor + m_ks * pow(value, m_shininess) * m_dirLightColors[i] * cSpecular;

    }

    for (int i = 0; i < m_numPointLights; i++) { // point lighting

        // attenuation
        float dist = distance(worldSpacePos, vec3(m_pointLightPos[i]));
        float f = min(1.f, 1.f / (m_pointLightAttenuation[i][0] + dist * m_pointLightAttenuation[i][1] + dist * dist * m_pointLightAttenuation[i][2]));

        // diffuse
        vec3 posToLightDir = normalize(vec3(m_pointLightPos[i]) - worldSpacePos);
        float dotDiffuse = clamp(dot(texturedNormal, posToLightDir), 0.f, 1.f);
        vec4 diffuse;
        if (textureMapping) { // check for texture mapping
            diffuse = dotDiffuse * textureColor;
        } else {
            diffuse = m_kd * dotDiffuse * cDiffuse;
        }
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
        vec4 diffuse;
        if (textureMapping) { // check for texture mapping
            diffuse = dotDiffuse * textureColor;
        } else {
            diffuse = m_kd * dotDiffuse * cDiffuse;
        }
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
    if (opacityMapping) { // check if opacity mapping on...
        vec4 opacity = texture(m_opacity_texture, obj_uv);
        fragColor[3] = opacity[0];
    } else {
        fragColor[3] = 1.f;
    }

    // water opacity
    if (m_water) {
        fragColor[3] = 0.95f;
    }

}

// --- student code end ---
