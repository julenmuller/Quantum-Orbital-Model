#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in float aIntensity;

uniform mat4 uProjection;
uniform mat4 uView;
uniform float uPointSize;

out vec3 vColor;
out float vIntensity;
out float vDepth;

void main() {
    vec4 viewPos = uView * vec4(aPos, 1.0);
    gl_Position = uProjection * viewPos;

    // Pontos maiores e melhor escala com distância
    float dist = max(length(viewPos.xyz), 0.5);
    gl_PointSize = max(uPointSize * 60.0 / dist, 2.5);

    vColor = aColor;
    vIntensity = aIntensity;
    vDepth = -viewPos.z;
}