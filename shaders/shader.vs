#version 410 core

#define N_MAX_DIR_LIGHT 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

uniform int nbDirLights;
uniform mat4 lightSpaceModels[N_MAX_DIR_LIGHT];


out vec3 normal;
out vec3 fragPosWorld;
out vec3 fragColor;
out vec4 fragPosLight[N_MAX_DIR_LIGHT];

void main(){
    gl_Position = projection * view * model * vec4(position, 1.0f);
//    gl_Position = vec4(position, 1.0f);
    fragColor = color;
    normal = normalize(inormal);
    fragPosWorld = vec3(model*vec4(position, 1));

    for(int i=0; i < nbDirLights; ++i) {
        fragPosLight[i] = lightSpaceModels[i] * vec4(fragPosWorld,1.0);
    }
}