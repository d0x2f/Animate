#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "Shader.hh"
#include "../Object/Property/Drawable.hh"
#include "../Object/Property/Movable.hh"
#include "../Object/Property/Scalable.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::GL
{
    class Quad : public Drawable, public Movable, public Scalable
    {
        public:
            Quad(Point position, Scale size);
            ~Quad();

            void set_texture_position(Position texture_position, Position texture_size = Position(1., 1.));

            void draw(Matrix model_matrix) override;

        protected:
            GLuint  vao_id = 0,
                    buffer_id = 0;
            Position texture_position;
            Position texture_size = Position(1., 1.);

            void initialise_buffers() override;
    };
}
