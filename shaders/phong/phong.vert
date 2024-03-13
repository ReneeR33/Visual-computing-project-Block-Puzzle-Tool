#version 410 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 iPosH = vec4(iPos, 1.0);

    FragPos = vec3(model * iPosH);
    Normal = mat3(transpose(inverse(model))) * iNormal;

    gl_Position = projection * view * model * iPosH;
}