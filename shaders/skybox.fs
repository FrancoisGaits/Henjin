#version 450

in vec3 texCoord;
layout (location = 0) out vec4 color;

uniform samplerCube gCubemapTexture;

void main()
{
    color = texture(gCubemapTexture, texCoord);
}