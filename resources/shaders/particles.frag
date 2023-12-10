#version 330 core
in vec4 ParticleColor;
out vec4 color;

//uniform sampler2D sprite;

void main()
{
    //color = (texture(sprite, TexCoords) * ParticleColor);
    color = vec4(1.0);
}

//in vec2 texcoordF;
//out vec4 outColor;

//uniform sampler2D tex;

//void main()
//{
//  outColor = texture(tex, texcoordF);
//}
