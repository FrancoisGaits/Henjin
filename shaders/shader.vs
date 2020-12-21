#version 460 core

#define N_MAX_DIR_LIGHT 4
#define N_MAX_BONES 16

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inormal;
layout (location = 2) in ivec4 bonesId;
layout (location = 3) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

uniform int nbDirLights;
uniform mat4 lightSpaceModels[N_MAX_DIR_LIGHT];
uniform mat4 bones[N_MAX_BONES];
uniform bool animGPU;

out vec3 normal;
out vec3 fragPosWorld;
out vec3 fragColor;
out vec4 fragPosLight[N_MAX_DIR_LIGHT];

void main(){
    fragPosWorld = vec3(model*vec4(position, 1.f));

    if(animGPU) {
        mat4 accV=mat4(0);

        for(int j=0; j<4 && bonesId[j] != -1; ++j) {
            accV += weights[j] * bones[bonesId[j]];
        }

        gl_Position = projection * view * accV * model * vec4(position, 1.0f);
        normal = normalize(transpose(inverse(accV * model)) * vec4(inormal,0)).xyz;

    } else {
        gl_Position =  projection * view *  model * vec4(position, 1.0f);
        normal = normalize(transpose(inverse(model)) * vec4(inormal,0)).xyz;
        fragColor = color;
    }

//    fragColor = color;


    for(int i=0; i < nbDirLights; ++i) {
        fragPosLight[i] = lightSpaceModels[i] * vec4(fragPosWorld,1.0);
    }
}