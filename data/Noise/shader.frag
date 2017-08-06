#version 450

layout (binding = 0) uniform variables {
    float random_seed;
} uniforms;

layout (location = 0) out vec4 out_colour;

// 2D Random
float random (in vec2 st) {
    return fract(
        sin(
            dot(
                st.xy,
                vec2(12.9898,78.233)
            )
        )
        * 43758.5453123
    );
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners porcentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main() {
    // Scale the coordinate system to see
    // some noise in action
    vec2 pos = gl_FragCoord.xy;

    // Use the noise function
    float r = noise(vec2(random(vec2(uniforms.random_seed+pos))));
    float g = noise(vec2(random(vec2(uniforms.random_seed+pos+vec2(1)))));
    float b = noise(vec2(random(vec2(uniforms.random_seed+pos-vec2(1)))));

    out_colour = vec4(r, g, b, 1.0);
}
