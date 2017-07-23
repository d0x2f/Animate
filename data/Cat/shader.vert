#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 colour;

layout (location = 1) out vec2 out_tex_coords;
layout (location = 3) out vec4 out_colour;

layout (std140) uniform matrices {
    mat4 model;
    mat4 projection;
    mat4 view;
};

void main() {
    out_colour = colour;
    out_tex_coords = tex_coords;

    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
