#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Object/Property/Drawable.hh"
#include "../Object/Property/Movable.hh"
#include "../Object/Property/Scalable.hh"

using namespace Animate::Geometry;
using namespace Animate::Object::Property;

namespace Animate::VK
{
    class Quad : public Drawable, public Movable, public Scalable
    {
        public:
            Quad(std::weak_ptr<VK::Context> context, Point position = Point(), Scale size = Scale(1.,1.,1.));
            ~Quad();

            void initialise_buffers() override;
            
            void set_texture_position(Position texture_position, Position texture_size = Position(1., 1.));
            std::vector< std::weak_ptr<Buffer> > const get_buffers() override;

            void set_model_matrix(Matrix model_matrix) override;

        protected:
            Position texture_position;
            Position texture_size = Position(1., 1.);

            std::weak_ptr<Buffer> vertex_buffer,
                                  index_buffer;

            void create_vertex_buffer();
            void create_index_buffer();
    };
}
