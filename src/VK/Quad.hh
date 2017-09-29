#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"
#include "../Geometry/Vertex.hh"
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

            void set_texture_position(Vector3 texture_position, Vector3 texture_size = Vector3(1., 1., 0.));

            vk::Buffer const get_vertex_buffer() override;
            vk::Buffer const get_index_buffer() override;

            void set_model_matrix(Matrix model_matrix) override;

        protected:
            Vector3 texture_position;
            Vector3 texture_size = Vector3(1., 1., 0.);

            std::weak_ptr<Buffer> vertex_buffer,
                                  index_buffer;

            void create_vertex_buffer();
            void create_index_buffer();
            void update_buffer();
            const std::vector<Vertex> get_data();
    };
}
