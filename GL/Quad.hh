#pragma once

#include <GL/glew.h>

#include "Definitions.hh"
#include "Matrix.hh"
#include "Shader.hh"

namespace Animate::GL
{
    class Quad
    {
        public:
            Quad(Point position, Size size, Colour colour);
            ~Quad();

            void initialise(Shader *shader, Texture *texture);
            void set_texture(Texture *texture);
            void set_shader(Shader *shader);
            void draw(Matrix model_matrix);

            Point get_position();
            void set_position(Point position);
            void move(Vector3 delta);

        protected:
            bool initialised = false;
            Point position;
            Size size;
            Colour colour;
            Shader *shader;
            Texture *texture;

            GLuint  vao_id = 0,
                    buffer_id = 0;

            void initialise_buffers();
    };
}
