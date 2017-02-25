#pragma once

#include <GL/glew.h>

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

            void draw(Matrix model_matrix);

        protected:
            GLuint  vao_id = 0,
                    buffer_id = 0;

            void initialise_buffers();
    };
}
