
// --- student code ---

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 coordinates;

out vec2 vertCoordinates;

void main() {
    vertCoordinates = coordinates;
    gl_Position = vec4(position, 1.0);
}

// --- student code end ---
