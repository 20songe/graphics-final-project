
// --- student code ---

#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in float material; // deprecated...

out vec3 worldSpacePos;
out vec3 worldSpaceNormal;
out vec2 obj_uv;
out vec2 textureCoord;
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

    // TODO: water ripple verts

}

const float tiling = 6.0;

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

    textureCoord = vec2(uv.x, uv.y) * tiling;

}



// --- student code end ---
