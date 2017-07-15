#version 450

layout (location = 0) in vec3 vertex;

layout (location = 0) out vec3 _vertex;
layout (location = 2) out vec4 _colour;

layout (std140) uniform variables {
    vec4 colour;
};

layout (std140) uniform matrices {
    mat4 model;
    mat4 projection;
    mat4 view;
};

void main() {
    _vertex = vertex;
    _colour = colour;

    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
