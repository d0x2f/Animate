#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 _vertex;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec4 _colour;
layout (location = 3) in vec2 _tex_coords;

out vec3 vertex;
out vec4 colour;
out vec2 tex_coords;

layout (std140) uniform matrices {
    mat4 model;
    mat4 projection;
    mat4 view;
};

void main() {
    vertex = _vertex;
    colour = _colour;
    tex_coords = _tex_coords;

    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
