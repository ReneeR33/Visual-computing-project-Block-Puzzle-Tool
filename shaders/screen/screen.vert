#version 420 core

out vec4 FragColor;

layout (binding = 0) uniform sampler2D screen;

void main()
{
    ivec2 coords = ivec2(gl_FragCoord.xy);
    vec4 color = texelFetch(screen, coords, 0);

    FragColor = vec4(color.rgb, 1.0);
}