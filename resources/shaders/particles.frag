#version 330 core
in vec2 TexCoords;
//in vec4 ParticleColor;
out vec4 color;

uniform sampler2D text;

void main()
{
    color = texture(text, TexCoords);
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}
