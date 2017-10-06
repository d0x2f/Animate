#version 450

layout (location = 0) in vec3 vertex;
layout (location = 0) out vec3 out_vertex;

layout (push_constant,row_major) uniform matrices {
    mat4 mvp;
} push_constants;

out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = push_constants.mvp * vec4(vertex, 1.0);
    out_vertex = vertex;
}
