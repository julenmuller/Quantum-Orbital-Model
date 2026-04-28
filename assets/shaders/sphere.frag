#version 330 core

in float vDepth;

out vec4 fragColor;

void main() {
    float depthFade = clamp(1.0 - vDepth * 0.015, 0.3, 1.0);
    fragColor = vec4(0.5, 0.55, 0.7, 0.4 * depthFade);
}