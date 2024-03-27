#version 420 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexcoords;
layout (location = 3) in vec3 iTangent;
layout (location = 4) in vec3 iBitTangent;

out vec3 FragPos;
out vec4 LightSpaceFragPos;
out vec3 Normal;
out vec2 Texcoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

// for rendering a scene inside the UI
uniform mat4 eTransform;

void main()
{
    vec4 iPosH = vec4(iPos, 1.0);

    FragPos = vec3(model * iPosH);
    LightSpaceFragPos = lightSpaceMatrix * model * iPosH;
    Normal = mat3(transpose(inverse(model))) * iNormal;
    Texcoords = -iTexcoords;

    vec3 T = normalize(vec3(model * vec4(iTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(iBitTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(iNormal,    0.0)));
    TBN = mat3(T, B, N);

    gl_Position = eTransform * projection * view * model * iPosH;
}