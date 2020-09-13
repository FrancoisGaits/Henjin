#version 410 core
in vec3 normal;
in vec3 fragPosWorld;

const float PI = 3.141592;
const float Epsilon = 0.00001;
const vec3 Fdielctric = vec3(0.04);

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

out vec4 color;

vec3 fresnel(vec3 F, float cos) {
    return F + (vec3(1)-F) * pow(1 - cos, 5);
}

float nd(float cos, float roughness) {
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float den = (cos * cos) * (alphaSq - 1) + 1;

    return alphaSq / (PI * den * den);
}

float gaG1(float cos, float k) {
    return cos / (cos * (1 - k) + k);
}

float ga(float cosNL, float cosNV, float roughness) {
    float r = roughness + 1;
    float k = (r*r) / 8;

    return gaG1(cosNL, k) * gaG1(cosNV, k);
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


    vec3 lightDir = lightPos - fragPosWorld;
    vec3 h = normalize(lightDir+view);

    float cosNL = max(0.0, dot(normal,lightDir));
    float cosNH = max(0.0, dot(normal, h));

    vec3 F = fresnel(F0, max(0.0, dot(h,view)));
    float D = nd(cosNH, roughness);
    float G = ga(cosNL, cosNV, roughness);


    vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

    vec3 diffuse = kd*albedo;

    vec3 specular = (D * F * G) / max(Epsilon, 4.0 * cosNL * cosNV);

    color = vec4((diffuse + specular) * lightColor * cosNL,1.0);

}