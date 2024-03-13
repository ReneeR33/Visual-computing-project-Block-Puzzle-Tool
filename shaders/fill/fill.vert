#version 410 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

uniform mat4 model;
uniform mat4 projection;

void main() {
    vec4 iPosH = vec4(iPos.x, iPos.y, 1, 1);
    gl_Position = projection * model * iPosH;
}