#version 450

layout (location = 1) in vec2 tex_coords;
layout (location = 3) in vec4 colour;

layout (location = 0) out vec4 output_colour;

layout(binding = 1) uniform sampler2D tex;

void main() {
    vec4 tex = texture(tex, tex_coords);
    output_colour = colour*tex;
}
