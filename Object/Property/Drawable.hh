#pragma once

#include <atomic>
#include <mutex>

#include "../../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate
{

    namespace VK
    {
        class Context;
        class Pipeline;
        class Texture;
        class Buffer;
    }

    namespace Object::Property
    {
        enum PrimitiveType {
            CONTAINER,
            QUAD,
            LINE,
            CIRCLE
        };

        class Drawable : public std::enable_shared_from_this<Drawable>
        {
            public:
                Drawable(std::weak_ptr<VK::Context> context, PrimitiveType type = CONTAINER, uint32_t indices = 0)
                    : context(context), type(type), indices(indices) {};
                virtual ~Drawable() {};

                void initialise(std::weak_ptr<VK::Pipeline> pipeline);
                void set_pipeline(std::weak_ptr<VK::Pipeline> pipeline);

                virtual vk::Buffer const get_vertex_buffer();
                virtual vk::Buffer const get_index_buffer();

                uint32_t get_index_count();

                std::weak_ptr<VK::Pipeline> const get_pipeline();
                Matrix const get_model_matrix();

                virtual void set_model_matrix(Matrix model_matrix);
                virtual void add_to_scene();
                virtual void initialise_buffers() = 0;

            protected:
                std::weak_ptr<VK::Context> context;

                std::mutex matrix_mutex;

                std::weak_ptr<VK::Pipeline> pipeline;
                Matrix model_matrix;

                bool initialised = false;
            
            private:
                PrimitiveType type;
                uint32_t indices;
                uint64_t pipeline_drawable_id;
        };
    }
}
