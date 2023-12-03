#version 330 core

// UV coordinate variable
in vec2 uv_out;

// Sampler2D variable
uniform sampler2D texSampler;

// Filtering texture
uniform bool filterBool;
uniform bool blurBool;
uniform bool grayBool;
uniform bool sharpenBool;
uniform bool sobel;

uniform float width;
uniform float height;

out vec4 fragColor;

void main()
{
    float pixelStepW = 1.0 / width;
    float pixelStepH = 1.0 / height;

    mat3 sobelx = mat3(-1, -2, -1,
                       0, 0, 0,
                       1, 2, 1);
    mat3 sobely = mat3(-1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1);
    // sampling for filter
    if (blurBool) {

        fragColor = vec4(0);
        for (int i = -2; i <= 2; i ++) {
            for (int j = -2; j <= 2; j++) {
                float u = float(i) * pixelStepW + uv_out[0];
                float v = float(j) * pixelStepH + uv_out[1];
                fragColor += texture(texSampler, vec2(u,v));
            }
        }

        fragColor = fragColor / 25;
    }
    else {
        fragColor = texture(texSampler, uv_out);
    }

    if (sharpenBool) {
        fragColor = vec4(0);
        for (int i = -1; i <= 1; i ++) {
            for (int j = -1; j <= 1; j++) {
                float u = float(i) * pixelStepW + uv_out[0];
                float v = float(j) * pixelStepH + uv_out[1];

                if (u == uv_out[0] && v == uv_out[1]) {
                    fragColor = fragColor + 17 * texture(texSampler, vec2(u,v));
                }
                else {
                    fragColor = fragColor - texture(texSampler, vec2(u,v));
                }


            }
        }

        fragColor = fragColor / 9;


    }


    if (sobel) {

        float yColor = 0;
        float xColor = 0;
        for (int i = -1; i <= 1; i ++) {
            for (int j = -1; j <= 1; j++) {
                float u = float(i) * pixelStepW + uv_out[0];
                float v = float(j) * pixelStepH + uv_out[1];

                vec4 color = texture(texSampler, vec2(u, v));
                float grayColor = 0.299*color.x+ 0.587*color.y + 0.114 * color.z;

                xColor += grayColor * sobelx[i + 1][j + 1];
                yColor += grayColor * sobely[i + 1][j + 1];

            }
        }
        fragColor = vec4(sqrt(pow(xColor, 2) + pow(yColor, 2)));
    }


    // Invert fragColor's r, g, and b color channels if your bool is true
    if (filterBool) {
        fragColor = vec4(1) - fragColor;
    }

    if (grayBool) {
        fragColor = vec4(0.299*fragColor.x+ 0.587*fragColor.y + 0.114 * fragColor.z);
    }
}
