#version 410 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexcoords;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitTangent;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(iPos, 1.0);
}