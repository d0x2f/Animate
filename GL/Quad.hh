#pragma once

#include <GL/glew.h>

#include "Definitions.hh"

namespace Animate::GL {

    class Quad
    {
        public:
            Quad(Point position, Size size, Colour colour);
            ~Quad();

            void initialise();
            void set_texture(GLint texture_id);
            void draw();

        protected:
            Point position;
            Size size;
            Colour colour;

        private:
            bool initialised = false;
            GLuint  vao_id = 0,
                    ibo_id = 0,
                    buffer_id = 0,
                    texture_id = 0,
                    shader_id = 0;

            void initialise_buffers();

    };

}
