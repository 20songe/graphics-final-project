#version 330 core

// UV coordinate variable
in vec2 uv_out;

// Sampler2D variable
uniform sampler2D texSampler;

uniform float width;
uniform float height;

out vec4 fragColor;

uniform sampler2D guiTexture;

void main()
{

//    fragColor = texture(texSampler, uv_out);
    fragColor = texture(guiTexture, uv_out);

}
