#version 140

in vec3 vertex;
in vec4 colour;
in vec2 tex_coords;

out vec4 output_colour;

uniform sampler2D tex0;

void main() {
    vec4 tex = texture(tex0, tex_coords);
    output_colour = colour*tex;
}
