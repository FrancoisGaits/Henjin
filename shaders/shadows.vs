#version 460 core

#define N_MAX_BONES 16

layout (location = 0) in vec3 position;
layout (location = 2) in ivec4 bonesId;
layout (location = 3) in vec4 weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform bool animGPU;
uniform mat4 bones[N_MAX_BONES];

void main(){
    if (animGPU) {
        mat4 accV=mat4(0);
        for(int j=0; j<4 && bonesId[j] != -1; ++j) {
            accV += weights[j] * bones[bonesId[j]];
        }
        gl_Position = lightSpaceMatrix * accV * model * vec4(position, 1.0f);
    } else {
        gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
    }
}