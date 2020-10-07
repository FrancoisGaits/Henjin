#version 410 core

#define N_MAX_POINT_LIGHT 16
#define N_MAX_DIR_LIGHT 4
#define EPSILON 0.01

struct PointLight {
    vec3 position;
    vec3 color;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

in vec3 normal;
in vec3 fragPosWorld;
in vec3 fragColor;
in vec4 fragPosLight[N_MAX_DIR_LIGHT];

const float PI = 3.141592;
const float Epsilon = 0.00001;
const vec3 Fdielctric = vec3(0.04);

uniform vec3 cameraPos;
uniform int nbPointLight;
uniform int nbDirectionalLight;
uniform int shadowMapSize;

uniform sampler2DShadow shadowMaps[N_MAX_DIR_LIGHT];

uniform PointLight pointLights[N_MAX_POINT_LIGHT];
uniform DirectionalLight directionalLights[N_MAX_DIR_LIGHT];



out vec4 color;

float shadow(vec4 fragPosLight, sampler2DShadow shadowMap) {
    // perform perspective divide
    vec3 projCoords = fragPosLight.xyz / fragPosLight.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xyz);
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//    // check whether current frag pos is in shadow
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    float Offset = 1.0/shadowMapSize;
    float yOffset = 1.0/shadowMapSize;

    float Factor = 0.0;

    for (int y = -1 ; y <= 1 ; y++) {
        for (int x = -1 ; x <= 1 ; x++) {
            vec2 Offsets = vec2(x * Offset, y * Offset);
            vec3 UVC = vec3(projCoords.xy + Offsets, projCoords.z - EPSILON);
            Factor += texture(shadowMap, UVC);
        }
    }

    return (Factor / 9.0);

    //return shadow;
}

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


    vec3 F0 = mix(Fdielctric, albedo, metalness);

    vec3 direct = vec3(0);
    vec3 totalcolor = vec3(0);

    //point lights
    for(int i=0; i<nbPointLight; ++i) {
        vec3 lightDir = pointLights[i].position - fragPosWorld;
        float lDist = length(lightDir);
        lightDir = lightDir/lDist;

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
        vec3 specular = (D * F * G) / max(Epsilon, 2.0 * cosNL * cosNV);

        totalcolor += pointLights[i].color;
        direct += (specular + ((vec3(1) - FT) * (vec3(1) - FTir) * lambertian) * dfc) * pointLights[i].color * fragColor * cosNL * (2/(lDist));
    }

    //directional
    for(int i=0; i<nbDirectionalLight; ++i) {
        vec3 lightDir = normalize(directionalLights[i].direction);
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
        vec3 specular = (D * F * G) / max(Epsilon, 2.0 * cosNL * cosNV);

        totalcolor += directionalLights[i].color;

        float sha = shadow(fragPosLight[i],shadowMaps[i]);

        direct += sha * clamp(((specular + ((vec3(1) - FT) * (vec3(1) - FTir) * lambertian) * dfc) * directionalLights[i].color * fragColor * cosNL),0.f,1.f);
    }

    vec3 ambiant = vec3(0.05) * fragColor * normalize(totalcolor);

    color = vec4(direct + ambiant,1.0);
}