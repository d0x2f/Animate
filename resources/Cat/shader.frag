#version 140
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

in vec3 vertex;
in vec4 colour;

layout (location = 3) in vec2 _tex_coords;

out vec4 output_colour;

uniform sampler2D tex0;

void main() {
    //vec4 tex = texture(tex0, tex_coords);
    output_colour = colour;
}
