#version 330 core

in vec3 vColor;
in float vIntensity;
in float vDepth;

out vec4 fragColor;

void main() {
    vec2 coord = gl_PointCoord - vec2(0.5);
    float dist2 = dot(coord, coord);
    if (dist2 > 0.25) discard;

    // Glow gaussiano mais suave
    float glow = exp(-dist2 * 8.0);

    // Depth fade mais sutil
    float depthFade = clamp(1.0 - vDepth * 0.008, 0.5, 1.0);

    // Cor mais brilhante (boost no brilho central)
    vec3 color = vColor * (0.6 + 0.7 * vIntensity) * depthFade;
    float alpha = glow * (0.5 + 0.5 * vIntensity);

    fragColor = vec4(color, alpha);
}