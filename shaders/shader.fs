#version 410 core

#define N_MAX_LIGHT 32

struct Light {
    vec3 position;
    vec3 color;
};

in vec3 normal;
in vec3 fragPosWorld;
in vec3 fragColor;

const float PI = 3.141592;
const float Epsilon = 0.00001;
const vec3 Fdielctric = vec3(0.04);

uniform vec3 cameraPos;
uniform int nbLight;

uniform Light lights[N_MAX_LIGHT];



out vec4 color;

vec3 fresnel(vec3 F, float cos) {
    return F + (vec3(1.0)-F) * pow(1.0 - cos, 5.0);
}

float nd(float cos, float roughness) {
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float den = (cos * cos) * (alphaSq - 1.0) + 1.0;

    return alphaSq / (PI * den * den);
}

float gaG1(float cos, float k) {
    return cos / (cos * (1.0 - k) + k);
}

float ga(float cosNL, float cosNV, float roughness) {
    float r = roughness + 1;
    float k = (r*r) / 8;

    return gaG1(cosNL, k) * gaG1(cosNV, k);
}

vec3 diffuseFresnel(vec3 ior) {
    vec3 iorSq = ior*ior;
    bvec3 TIR = lessThan(ior, vec3(1.0));
    vec3 invdenum = mix(vec3(1.0), vec3(1.0) / (iorSq * iorSq * (vec3(554.33) - 380.7 * ior)), TIR);
    vec3 num = ior * mix(vec3(0.1921156102251088), ior * 298.25 - 261.38 * iorSq + 138.43, TIR);
    num += mix(vec3(0.8078843897748912), vec3(-1.67), TIR);
    return num * invdenum;
}

void main() {
//    color = vec4( clamp( dot( normalize(normal), normalize(lightPos-fragPosWorld)), 0, 1 ) * lightColor, 1.0);

    vec3 albedo = vec3(1);
    float metalness = 1;
    float roughness = 0.5;

    vec3 view = normalize(cameraPos - fragPosWorld);

    float cosNV = max(0.0,dot(normal,view));

    vec3 spec = 2.0*view*normal - view;

    vec3 F0 = mix(Fdielctric, albedo, metalness);

    vec3 direct = vec3(0);
    for(int i=0; i<nbLight; ++i) {
        vec3 lightDir = lights[i].position - fragPosWorld;
        vec3 h = normalize(lightDir+view);

        float cosNL = max(0.0, dot(normal, lightDir));
        float cosNH = max(0.0, dot(normal, h));

        vec3 F = fresnel(F0, max(0.0, dot(h, view)));
        float D = nd(cosNH, roughness);
        float G = ga(cosNL, cosNV, roughness);

        vec3 FT = fresnel(F0, max(0.0, dot(normal,lightDir)));
        vec3 FTir = fresnel(F0, max(0.0, dot(view, normal)));

        vec3 dfc = diffuseFresnel(vec3(1.05));

        vec3 lambertian = albedo / PI;


//        vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);
//
//        vec3 diffuse = kd*albedo;

        vec3 specular = (D * F * G) / max(Epsilon, 4.0 * cosNL * cosNV);

//        direct += vec3((diffuse + specular) * lights[i].color * cosNL);

        direct += (specular + ((vec3(1) - FT) * (vec3(1) - FTir) * lambertian) * dfc) * lights[i].color * fragColor * cosNL;
    }

    color = vec4(direct,1.0);
}