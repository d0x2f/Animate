#version 450

layout (location = 0) in vec3 vertex;
layout (location = 2) in vec4 colour;
layout (location = 3) in vec2 tex_coords;

layout (location = 2) out vec4 output_colour;

uniform sampler2D tex0;

void main() {
    vec4 tex = texture(tex0, tex_coords);
    output_colour = colour*tex;
}
