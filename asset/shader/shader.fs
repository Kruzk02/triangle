#version 330 core

out vec4 fragColor;
uniform float uTime;

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float interval = 2.0;
    float t = mod(uTime, interval) / interval;

    float seed1 = floor(uTime / interval);
    float seed2 = seed1 + 1.0;

    vec3 colorA = vec3(
        rand(vec2(seed1, 1.0)),
        rand(vec2(seed1, 2.0)),
        rand(vec2(seed1, 3.0))
    );

    vec3 colorB = vec3(
        rand(vec2(seed2, 1.0)),
        rand(vec2(seed2, 2.0)),
        rand(vec2(seed2, 3.0))
    );

    vec3 color = mix(colorA, colorB, t);

    fragColor = vec4(color, 1.0);
}
