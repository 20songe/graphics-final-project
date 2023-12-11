#version 330 core
in vec2 TexCoords;
//in vec4 ParticleColor;
out vec4 color;

uniform vec3 leaf_shade;
uniform sampler2D leaf_color;
uniform sampler2D normal;
uniform sampler2D opacity;

vec3 rgb2hsl(vec3 rgb) {
    float r = rgb[0];
    float g = rgb[1];
    float b = rgb[2];

    float maximum = max(max(r, g), b);
    float minimum = min(min(r, g), b);

    float hue;
    float sat;
    float lum = (maximum + minimum) / 2.f;

    if (maximum == minimum) { // no saturation
        hue = 0.f;
        sat = 0.f;
    } else {

        float c = maximum - minimum; // chroma
        sat = c / (1.f - abs(2.f * lum - 1.f));

        if (maximum == r) {
            hue = (g - b) / c;
        } else if (maximum == g) {
            hue = (b - r) / c + 2.f;
        } else {
            hue = (r - g) / c + 4.f;
        }

    }
    hue = hue / 6.f;
    if (hue < 0.f) hue += 1.f;

    return vec3(hue, sat, lum); // hue, sat, lum

}

float hue2rgb(float p, float q, float t) {
  if (t < 0.f) t += 1.f;
  if (t > 1.f) t -= 1.f;
  if (t < 1.f / 6.f) return p + (q - p) * 6.f * t;
  if (t < 1.f / 2.f) return q;
  if (t < 2.f / 3.f) return p + (q - p) * (2.f / 3.f - t) * 6.f;
  return p;
}

vec3 hsl2rgb(vec3 hsl) { // all inputs from [0, 1]
  float r;
  float g;
  float b;

  if (hsl[1] == 0) { // no saturation
    r = hsl[2];
    g = hsl[2];
    b = hsl[2];
  } else {

    float q = 0.f;
    if (hsl[2] < 0.5f) {
        q = hsl[2] * (1 + hsl[1]);
    } else {
        q = hsl[2] + hsl[1] - hsl[2] * hsl[1];
    }

    float p = 2.f * hsl[2] - q;

    r = hue2rgb(p, q, hsl[0] + 1.f / 3.f);
    g = hue2rgb(p, q, hsl[0]);
    b = hue2rgb(p, q, hsl[0] - 1.f / 3.f);

  }

  return vec3(r, g, b);

}

//void saturate() {

//    vec3 hsl = rgb2hsl(vec3(fragColor));
//    hsl[1] *= 2.f;
//    fragColor = vec4(hsl2rgb(hsl), 1.f);

//}

//void hue() {

//    vec3 hsl = rgb2hsl(vec3(fragColor));
//    hsl[0] *= 2.f;
//    fragColor = vec4(hsl2rgb(hsl), 1.f);

//}


void main()
{
    color = texture(leaf_color, TexCoords);

//    vec3 hsl = rgb2hsl(vec3(color));
//    hsl[0] *= 6.f;
//    color = vec4(hsl2rgb(hsl), 1.f);
//    color[0] += 0.5;
    //color[0] += 0.2;
//    color[1] -= 0.2;
//    color[2] -= 0.5;
//    color[0] += 0.2;



    //color = vec4(1.0, 0.0, 0.0, 1.0);

    vec4 opacity_value = texture(opacity, TexCoords);
    color[3] = opacity_value[0];
    color += vec4(leaf_shade, 0.f);

}
