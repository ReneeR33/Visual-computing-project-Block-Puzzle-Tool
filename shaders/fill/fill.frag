#version 410 core

out vec4 FragColor;

in vec2 FragPos;

in vec2 Top;
in vec2 Right;
in vec2 Center;

uniform vec3 color;
uniform float offset = 20.0;

void main() {
    vec2 upVec = (Top - Center);
    vec2 rightVec = (Right - Center);

    vec2 leftTopEdge = (Center + upVec - rightVec - normalize(upVec) * offset + normalize(rightVec) * offset);
    vec2 leftBottomEdge = (Center - upVec - rightVec + normalize(upVec) * offset + normalize(rightVec) * offset);
    vec2 rightTopEdge = (Center + upVec + rightVec - normalize(upVec) * offset - normalize(rightVec) * offset);
    vec2 rightBottomEdge = (Center - upVec + rightVec + normalize(upVec) * offset - normalize(rightVec) * offset);

    if ((dot(FragPos - leftTopEdge, upVec) > 0) &&
        (dot(FragPos - leftTopEdge, rightVec) < 0) &&
        (pow(FragPos.x - leftTopEdge.x, 2) + pow(FragPos.y - leftTopEdge.y, 2) > pow(offset, 2))) {
            discard;
    }

    if ((dot(FragPos - leftBottomEdge, upVec) < 0) &&
        (dot(FragPos - leftBottomEdge, rightVec) < 0) &&
        (pow(FragPos.x - leftBottomEdge.x, 2) + pow(FragPos.y - leftBottomEdge.y, 2) > pow(offset, 2))) {
            discard;
    }

    if ((dot(FragPos - rightTopEdge, upVec) > 0) &&
        (dot(FragPos - rightTopEdge, rightVec) > 0) &&
        (pow(FragPos.x - rightTopEdge.x, 2) + pow(FragPos.y - rightTopEdge.y, 2) > pow(offset, 2))) {
            discard;
    }

    if ((dot(FragPos - rightBottomEdge, upVec) < 0) &&
        (dot(FragPos - rightBottomEdge, rightVec) > 0) &&
        (pow(FragPos.x - rightBottomEdge.x, 2) + pow(FragPos.y - rightBottomEdge.y, 2) > pow(offset, 2))) {
            discard;
    }

    FragColor = vec4(color, 1);
}