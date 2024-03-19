#version 410 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(iPos, 1.0);
}