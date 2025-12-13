#version 330 core

out vec4 fragColor;

in vec2 vUV;

uniform float uTime;
uniform sampler2D uTexture;

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float interval = 2.0;
    float seed = floor(uTime / interval);
    float t = mod(uTime, interval) / interval;

    vec3 topA = vec3(
        rand(vec2(seed, 1.0)),
        rand(vec2(seed, 2.0)),
        rand(vec2(seed, 3.0))
    );

    vec3 topB = vec3(
        rand(vec2(seed + 1.0, 1.0)),
        rand(vec2(seed + 1.0, 2.0)),
        rand(vec2(seed + 1.0, 3.0))
    );

    vec3 bottomA = vec3(
        rand(vec2(seed + 2.0, 1.0)),
        rand(vec2(seed + 2.0, 2.0)),
        rand(vec2(seed + 2.0, 3.0))
    );

    vec3 bottomB = vec3(
        rand(vec2(seed + 3.0, 1.0)),
        rand(vec2(seed + 3.0, 2.0)),
        rand(vec2(seed + 3.0, 3.0))
    );

    vec3 topColor = mix(topA, topB, t);
    vec3 bottomColor = mix(bottomA, bottomB, t);

    float gradient = gl_FragCoord.y / 720.0;
    vec3 proceduralColor = mix(bottomColor, topColor, gradient);

    vec3 textureColor = texture(uTexture, vUV).rgb;

    vec3 finalColor = proceduralColor * textureColor;

    fragColor = texture(uTexture, vUV);
}