#version 330

layout (location = 0) in vec3 _vertex;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec4 _colour;
layout (location = 3) in vec2 _tex_coords;

out vec3 vertex;
out vec4 colour;
out vec2 tex_coords;

layout (row_major,std140) uniform matrices {
    mat4 modelviewprojection;
    mat4 modelview;
};

void main() {
    vertex = _vertex;
    colour = _colour;
    tex_coords = _tex_coords;

    gl_Position = modelviewprojection * vec4(vertex, 1.0);
}
