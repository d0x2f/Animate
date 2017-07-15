#version 450

layout (location = 0) in vec3 _vertex;
layout (location = 2) in vec4 _colour;

layout (location = 0) out vec4 output_colour;

void main() {
    output_colour = _colour;
}
