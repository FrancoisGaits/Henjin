#version 460 core
layout (location = 0) out vec4 color;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBuffer;

uniform bool bloom;
uniform int toneMapping;

float luminance(vec3 v)
{
    return dot(v, vec3(0.2126f, 0.7152f, 0.0722f));
}


vec3 tonemapFilmic(const vec3 color) {
    vec3 x = max(vec3(0.0), color - 0.004);
    return (x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06);
}


vec3 changeLuminance(vec3 c_in, float l_out){
    float l_in = luminance(c_in);
    return c_in * (l_out / l_in);
}

vec3 tonemapFilmic_lumin(const vec3 color) {
    float lum = luminance(color);
    float x = max(0.0, lum - 0.004);
    return clamp(changeLuminance(color,(x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06)),0,1);
}

void main() {
    vec3 hdr = texture(hdrBuffer, TexCoords).rgb;

    vec3 mapped;
    switch(toneMapping){
        case 1:
            mapped = tonemapFilmic(hdr);
            break;
        case 2:
            mapped = tonemapFilmic_lumin(hdr);
            break;
        case 3:
            mapped = hdr/ (vec3(1) + hdr);
            break;
        case 0:
        default:
            mapped = hdr;
            break;
    }

    if(bloom) {
        vec3 bloom_color = texture(bloomBuffer, TexCoords).rgb;

        mapped += bloom_color;
    }




    mapped = pow(mapped, vec3(1.f/2.2));
    color = vec4(mapped, 1);


}