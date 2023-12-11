#version 330 core
in vec2 coordinate;

//uniform bool toInvert;
//uniform bool toBlur;
//uniform bool toGray; // extra credit 1

uniform float deltaU;
uniform float deltaV;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    fragColor = vec4(1);
    fragColor = texture(tex, coordinate);

//    if(toInvert) {
//        fragColor = 1 - fragColor;
//    }

//    if (toGray) {
//        fragColor = vec4(vec3((fragColor[1] + fragColor[2] + fragColor[3])/3), 1.0f);
//    }

//    vec3 average = vec3(0);
//    if(toBlur) {
//        for (int i = -2; i <= 2; i++) {
//            for (int j = -2; j <= 2; j++) {
//                vec2 neighborPixel =  vec2(coordinate.x + (deltaU * i), coordinate.y + (deltaV * j));
//                average += texture(tex, neighborPixel).xyz;
//            }
//        }
//        fragColor = vec4(average/25, 1.0f);

//        if(toInvert) {
//            fragColor = 1 - fragColor;
//        }

//        if (toGray) {
//            fragColor = vec4(vec3((fragColor[1] + fragColor[2] + fragColor[3])/3), 1.0f);
//        }
//    }
}
