#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoord;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(Pos.x, Pos.y, Pos.z, 1.0);
    TexCoords = TexCoord;
}