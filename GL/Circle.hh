#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Object/Property/Drawable.hh"
#include "../Object/Property/Movable.hh"
#include "../Object/Property/Scalable.hh"
#include "../Object/Property/Coloured.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::GL
{
    class Circle : public Drawable, public Movable, public Scalable, public Coloured
    {
        public:
            Circle(Point position, Scale size, Colour colour, GLfloat thickness);
            ~Circle();

            void draw(Matrix model_matrix) override;

        protected:
            GLuint  vao_id = 0,
                    buffer_id = 0;
            GLfloat thickness;

            void initialise_buffers() override;
    };
}
