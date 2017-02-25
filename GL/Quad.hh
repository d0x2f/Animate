#pragma once

#include <GL/glew.h>

#include "Definitions.hh"
#include "Matrix.hh"
#include "Shader.hh"
#include "Drawable.hh"
#include "Movable.hh"
#include "Scalable.hh"

namespace Animate::GL
{
    class Quad : public Drawable, public Movable, public Scalable
    {
        public:
            Quad(Point position, Scale size);
            ~Quad();

            void draw(Matrix model_matrix);

        protected:
            GLuint  vao_id = 0,
                    buffer_id = 0;

            void initialise_buffers();
    };
}
