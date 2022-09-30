#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    TexCoords = aTexCoords;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
    vec4 texture1 = texture(tex1, TexCoords);
    vec4 texture2 = texture(tex2, TexCoords);

    vec4 finalColor = texture1 * texture2 * 2.0f;

    FragColor = finalColor;
}