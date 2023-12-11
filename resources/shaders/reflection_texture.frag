#version 330 core

// UV coordinate variable
in vec2 uv_out;

// Sampler2D variable
uniform sampler2D texSampler;

uniform float width;
uniform float height;

out vec4 fragColor;

void main()
{

    fragColor = texture(texSampler, uv_out);

}
