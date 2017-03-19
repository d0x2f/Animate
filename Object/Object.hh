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
            Object(Point position = Point(), Scale size = Scale(1.,1.,1.));
            ~Object();

            void initialise();
            void add_component(Drawable *component);
            void clear_components();
            virtual void draw(Matrix model_matrix);

            virtual void on_tick(GLuint64 time_delta);

        protected:
            std::vector< std::shared_ptr<Drawable> > components;

            void initialise_buffers();
    };
}

