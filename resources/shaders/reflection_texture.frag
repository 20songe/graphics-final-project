#version 330 core

// UV coordinate variable
in vec2 uv_out;
in vec3 vertPos;
in vec3 out_normal;
in float obj_index;

// Sampler2D variable
uniform sampler2D texSampler;

uniform float width;
uniform float height;
//uniform mat4 reflection_projection;

out vec4 fragColor;

void main()
{

    int int_obj = int(obj_index);
    if (int_obj == 2) {
        fragColor = texture(texSampler, uv_out);
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (int_obj == 1) {
        fragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
//    fragColor = vec4(0.0, 0.0, 1.0, 1.0);


//    vec4 vClipReflection = reflection_projection * vec4(vertPos.x,0.0, vertPos.z , 1.0);
//    vec2 vDeviceReflection = vClipReflection.st / vClipReflection.q;
//    vec2 vTextureReflection = vec2(0.5, 0.5) + 0.5 * vDeviceReflection;

//    vec4 reflectionTextureColor = texture(texSampler, vTextureReflection);

//    // Framebuffer reflection can have alpha > 1
//    reflectionTextureColor.a = 1.0;

//    fragColor = reflectionTextureColor;
//    fragColor = vec4(1.0, 0.0, 0.0, 1.0);


}
