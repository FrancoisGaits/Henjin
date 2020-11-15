#version 450

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoord;

void main()
{
    vec4 worldPos = projection * view * vec4(position, 1.0);
    gl_Position = worldPos.xyww;
    texCoord = position;
}