#version 460 core
out vec4 color;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;

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
    return changeLuminance(color,(x * (6.2 * x + 0.5)) / (x * (6.2 * x + 1.7) + 0.06));
}

void main() {
    vec3 hdr = texture(hdrBuffer, TexCoords).rgb;
    //    vec3 mapped = tonemapFilmic_lumin(hdr);


    if(luminance(hdr)>0.0f) {

        //    vec3 mapped = tonemapFilmic(hdr);
        vec3 mapped = tonemapFilmic_lumin(hdr);
        //    vec3 mapped = hdr/ (vec3(1) + hdr);

        mapped = pow(mapped, vec3(1.f/2.2));
        color = vec4(mapped, 1);
    } else {
        color = vec4(0,0,0,1);
    }

}