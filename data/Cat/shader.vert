#version 450
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 colour;

layout (push_constant,row_major) uniform matrices {
    mat4 mvp;
} push_constants;

out gl_PerVertex {
    vec4 gl_Position;
};

layout (location = 1) out vec2 out_tex_coords;
layout (location = 3) out vec4 out_colour;

void main() {
    out_colour = colour;
    out_tex_coords = tex_coords;

    gl_Position = push_constants.mvp * vec4(vertex, 1.0);
}
