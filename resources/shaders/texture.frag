#version 330 core

// Create a UV coordinate in variable
in vec2 fragUV;

// Add a sampler2D uniform
uniform sampler2D textureSampler;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    // Set fragColor using the sampler2D at the UV coordinate
    fragColor = texture(textureSampler, fragUV);
}
