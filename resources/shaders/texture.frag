#version 330 core
in vec2 coordinate;

uniform float deltaU;
uniform float deltaV;

uniform sampler2D tex;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

out vec4 fragColor;

void main(){

    vec4 reflectionColor = texture(reflectionTexture, coordinate);
    vec4 refractColor = texture(refractionTexture, coordinate);
    fragColor = vec4(1);
//    fragColor = texture(tex, coordinate);
    fragColor = mix(reflectionColor, refractColor, 0.5);
}
