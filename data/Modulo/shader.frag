#version 140

in vec3 _vertex;
in vec4 _colour;

out vec4 output_colour;

void main() {
    output_colour = _colour;
}
