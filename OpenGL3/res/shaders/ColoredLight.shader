#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform vec4 objectColor;
uniform vec4 lightColor;

uniform sampler2D tex1;

void main()
{
    FragColor = objectColor * lightColor;
}