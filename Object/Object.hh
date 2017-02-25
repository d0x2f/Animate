#pragma once

#include <vector>
#include <memory>

#include "Property/Drawable.hh"
#include "Property/Movable.hh"
#include "Property/Scalable.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Object::Property;

namespace Animate::Object
{
    class Object : public Movable, public Scalable, public Drawable
    {
        public:
            Object(Point position, Scale size);

            void initialise();
            void add_component(Drawable *component);
            void draw(Matrix model_matrix);

            virtual void on_tick(GLuint64 time_delta) = 0;

        protected:
            std::vector< std::unique_ptr<Drawable> > components;

            void initialise_buffers();
    };
}

