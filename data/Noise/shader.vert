#version 450

layout (location = 0) in vec3 vertex;

layout (location = 0) out vec3 out_vertex;
layout (location = 3) out vec4 out_colour;

layout (std140) uniform variables {
    vec4 colour;
};

layout (std140) uniform matrices {
    mat4 model;
    mat4 projection;
    mat4 view;
};

void main() {
    out_vertex = vertex;
    out_colour = colour;

    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
