#version 450

layout (location = 3) in vec4 colour;

layout (binding = 0) uniform variables {
    float hue;
} uniforms;

layout (location = 0) out vec4 out_colour;

void main() {

    //Convert the hue to an rgb value asuming saturation and value are 100%.
    float x = 1 - abs(mod(uniforms.hue, 2.) - 1);
    if (uniforms.hue <= 1) {
        out_colour = vec4(1., x, 0., 1.);
    } else if (uniforms.hue <= 2) {
        out_colour = vec4(x, 1., 0., 1.);
    } else if (uniforms.hue <= 3) {
        out_colour = vec4(0., 1, x, 1.);
    } else if (uniforms.hue <= 4) {
        out_colour = vec4(0, x, 1., 1.);
    } else if (uniforms.hue <= 5) {
        out_colour = vec4(x, 0., 1., 1.);
    } else if (uniforms.hue <= 6) {
        out_colour = vec4(1, 0., x, 1.);
    }
}
