#version 460 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec2 iTexCoord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(iPos,1);
    TexCoords = iTexCoord;
}