#version 410 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;

uniform mat4 model;
uniform mat4 projection;

out vec2 FragPos;

out vec2 Top;
out vec2 Right;
out vec2 Center;

void main() {
    vec4 iPosH = vec4(iPos.x, iPos.y, 1, 1);

    FragPos = vec2(model * iPosH);
    Top = vec2(model * vec4(0,0.5,1,1));
    Right = vec2(model * vec4(0.5,0,1,1));
    Center = vec2(model * vec4(0,0,1,1));

    gl_Position = projection * model * iPosH;
}