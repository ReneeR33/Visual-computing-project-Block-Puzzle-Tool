#version 420 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexcoords;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitTangent;

out vec2 Texcoords;

void main()
{
    Texcoords = iTexcoords;

    vec4 iPosH = vec4(iPos, 1.0);
    gl_Position = iPosH;
}