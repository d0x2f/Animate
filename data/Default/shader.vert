#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 vertex;
layout(location = 3) in vec3 colour;

layout(location = 3) out vec3 out_colour;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = vec4(vertex, 0.0, 1.0);
    out_colour = colour;
}
