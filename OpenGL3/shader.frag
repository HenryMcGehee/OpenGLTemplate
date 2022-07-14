#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;

void main()
{
    FragColor = vec4(1, 1, 1, 1);
}