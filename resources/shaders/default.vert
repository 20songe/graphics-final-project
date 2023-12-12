
// --- student code ---

#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in float material;

out vec3 worldSpacePos;
out vec3 worldSpaceNormal;
out vec2 obj_uv;
out float materialIndex;

uniform mat4 m_model;
uniform mat4 m_modelInverse;

uniform mat4 m_view;
uniform mat4 m_proj;

uniform vec3 m_offset;

uniform bool m_water;
uniform int m_numWaterPoints;
uniform vec4[8] m_waterPointCenters;
uniform float[8] m_waterPointElapsedTimes;


void waterRipples() {

    // general information
    obj_uv = uv;
    materialIndex = material; // no materials on the material index??? or a basic water texture...
    worldSpacePos = vec3(m_model * vec4(vertex, 1.f));
    worldSpaceNormal = mat3(m_model) * normal;
    gl_Position = m_proj * m_view * (m_model * vec4(vertex, 1.f) + vec4(m_offset, 1.f));

    // water ripples
    float strength = 1.f;
    float movementSpeed = 1.f;

    for (int i = 0; i < m_numWaterPoints; i++) {

        // distance from water point center...
        float dist = distance(vec3(m_waterPointCenters[i]), worldSpacePos);

        // ripple value, from [0, 2 * strength]
        float amplitude = strength * sin((dist - m_waterPointElapsedTimes[i]) * movementSpeed * 2.f * 3.14f) + strength;

        // normal calculation
        vec3 direction = worldSpacePos - vec3(m_waterPointCenters[i]);
        direction[1] = 0.f;
        direction = normalize(direction) + vec3(1.f);

        float da = cos((dist - m_waterPointElapsedTimes[i]) * movementSpeed * 2.f * 3.14f);

        worldSpaceNormal = normalize(direction * da + worldSpaceNormal);

        break;

    }

    // ge delta time in here...
    // how o get the ripping effect... use sin on the distance??




}

void main() {

    if (!m_water) {
        obj_uv = uv;
        materialIndex = material;

        worldSpacePos = vec3(m_model * vec4(vertex, 1.f));
        worldSpaceNormal = mat3(m_model) * normal;
        gl_Position = m_proj * m_view * (m_model * vec4(vertex, 1.f) + vec4(m_offset, 1.f));
    } else {
        waterRipples();
    }

}



// --- student code end ---
