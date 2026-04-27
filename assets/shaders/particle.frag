#version 330 core

in vec3 vColor;
in float vIntensity;

out vec4 fragColor;

void main() {
    vec3 color = vColor * (0.5 + 0.5 * vIntensity);
    fragColor = vec4(color, 1.0);
}
