#version 420 core

out vec4 FragColor;

in vec2 Texcoords;

layout (binding = 0) uniform sampler2D screen;

void main()
{
    vec4 color = texture(screen, Texcoords);

    FragColor = vec4(color.xyz, 1.0);
}