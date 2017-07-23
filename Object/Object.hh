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
            Object(std::weak_ptr<VK::Context> context, Point position = Point(), Scale size = Scale(1.,1.,1.));
            ~Object();

            void initialise();
            void add_component(std::shared_ptr<Drawable> component);
            void clear_components();
            virtual void set_model_matrix(Matrix model_matrix);
            virtual void add_to_scene();

            virtual void on_tick(GLuint64 time_delta);

        protected:
            std::vector< std::shared_ptr<Drawable> > components;

            void initialise_buffers();
    };
}

