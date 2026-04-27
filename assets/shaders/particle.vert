#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in float aIntensity;

uniform mat4 uProjection;
uniform mat4 uView;
uniform float uPointSize;

out vec3 vColor;
out float vIntensity;

void main() {
    gl_Position = uProjection * uView * vec4(aPos, 0.0, 1.0);
    gl_PointSize = max(uPointSize * 2.0, 4.0);
    vColor = aColor;
    vIntensity = aIntensity;
}
